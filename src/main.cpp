#include <Arduino.h>
#include <EEPROM.h>            // Include EEPROM library
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP.h>

#include "credentials.h" // This is the file where the credentials are stored

#include "converter.h"     // Include the external converter functions

#include "network.h"       // Include the external network functions
#include "system.h"        // Include the external system functions

#include "html/landing_page.h"  // Include the external landing page function
#include "html/landing_page_test.h"        // Include the external configuration submit page function
#include "html/saved.h"         // Include the external configuration saved page function

#include <Adafruit_NeoPixel.h>
#define PIN D2
#define NUMPIXELS 8

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

const uint32_t COLOR_RED = strip.Color(255, 0, 0);    // Red
const uint32_t COLOR_GREEN = strip.Color(0, 255, 0);  // Green
const uint32_t COLOR_BLUE = strip.Color(0, 0, 255);   // Blue
const uint32_t COLOR_YELLOW = strip.Color(255, 255, 0); // Yellow
const uint32_t COLOR_WHITE = strip.Color(255, 255, 255); // White
const uint32_t COLOR_BLACK = strip.Color(0, 0, 0);     // Off (black)


// Web server object
ESP8266WebServer server(80);

// Configuration variables
String deviceName = "D1 Mini";
String wifiSSID = "";
String wifiPassword = "";
bool enableLED = false;
bool enableLogging = false;
bool enableAlerts = false;
int eeprom_space = 0;
int eeprom_max_space = 0;
int strip_brightness = 50;

const int ledPin = D4;         // Onboard LED pin (GPIO2)

// get free eeprom space
// https://arduino.stackexchange.com/questions/1013/how-do-i-find-out-how-much-eeprom-memory-i-have-left

void getFreeEepromSpace() {
    size_t i; // Change the type of i to size_t
    int freeSpace = 0;

    for (i = 0; i < EEPROM.length(); i++) {
        if (EEPROM.read(i) == 0xFF) {
            freeSpace++;
        }
    }

    Serial.print("Free EEPROM space: ");
    Serial.print(freeSpace);
    Serial.println(" bytes");
}

void getMaxEepromSpace() {
  eeprom_max_space = EEPROM.length();
  Serial.print("Max EEPROM space: ");
  Serial.print(EEPROM.length());
  Serial.println(" bytes");
}

// Function to save the LED state to EEPROM
void saveLEDState(bool state) {
  EEPROM.write(0, state);      // Write LED state to EEPROM at address 0
  EEPROM.commit();             // Commit changes to EEPROM
}

// Function to load the LED state from EEPROM
bool loadLEDState() {
  return EEPROM.read(0) == 1;  // Read LED state from EEPROM
}

void saveBrightness(int brightness) {
  EEPROM.write(1, brightness); // Use EEPROM.update to write brightness to EEPROM at address 1
  EEPROM.commit();             // Commit changes to EEPROM
}

int loadBrightness() {
  return EEPROM.read(1); // Read brightness from EEPROM
}

// Handle the root URL and serve the landing page
void handleRoot() {
  server.send(200, "text/html", getLandingPage(

    enableLED, 
    enableLogging, 
    enableAlerts, 
    get_ip_address(), 
    get_mac_address(), 
    get_ssid(), 
    get_hostname(), 
    eeprom_space, 
    eeprom_max_space, 
    get_free_heap(), 
    strip_brightness)
    );
}

void handleRoot2() {
  server.send(200, "text/html", getLandingPageTest(

    enableLED, 
    enableLogging, 
    enableAlerts, 
    get_ip_address(), 
    get_mac_address(), 
    get_ssid(), 
    get_hostname(), 
    eeprom_space, 
    eeprom_max_space, 
    get_free_heap(), 
    strip_brightness)
    );
}

// Handle form submissions
void handleFormSubmit() {
  if (server.hasArg("deviceName")) deviceName = server.arg("deviceName");
  if (server.hasArg("wifiSSID")) wifiSSID = server.arg("wifiSSID");
  if (server.hasArg("wifiPassword")) wifiPassword = server.arg("wifiPassword");
  
  enableLED = server.hasArg("enableLED");   // Save LED state based on the checkbox

  strip_brightness = server.arg("brightness").toInt(); // Get the brightness value from the form
  strip.setBrightness(strip_brightness); // Set the brightness from 0 to 255
  strip.show();
  
  saveLEDState(enableLED);                  // Save the LED state to EEPROM
  saveBrightness(strip_brightness); // Save the brightness to EEPROM

  digitalWrite(ledPin, enableLED ? LOW : HIGH);  // Turn LED on/off based on the state (LOW is ON for onboard LED)

  enableLogging = server.hasArg("enableLogging");
  enableAlerts = server.hasArg("enableAlerts");

  server.send(200, "text/html", getConfigurationSavedPage());
}

void handleFormUpdate() {

  if (server.hasArg("enableLED")) {
      bool resp = stringToBool(server.arg("enableLED"));
      saveLEDState(resp); 
      digitalWrite(ledPin, resp ? LOW : HIGH);

  }
  if (server.hasArg("enableLogging")) {
      bool resp = stringToBool(server.arg("enableLogging"));
      enableLogging = resp;

      if (enableLogging) {
        Serial.println("Logging enabled");
        strip.setPixelColor(0, COLOR_GREEN);
      } else {
        Serial.println("Logging disabled");
        strip.setPixelColor(0, COLOR_BLACK);
      }
      strip.show();
      Serial.print("enableLogging: ");
      Serial.println(enableLogging);
      }
  if (server.hasArg("enableAlerts")) {
      bool resp = stringToBool(server.arg("enableAlerts"));
      enableAlerts = resp;
      Serial.print("enableAlerts: ");
      Serial.println(enableAlerts);
      }
  if (server.hasArg("brightness")) {
      strip_brightness = server.arg("brightness").toInt(); // Get the brightness value from the form
      strip.setBrightness(strip_brightness); // Set the brightness from 0 to 255
      strip.show();
      }

    // Update the global variables
    ::enableLED = enableLED;
    ::enableLogging = enableLogging;
    ::enableAlerts = enableAlerts;

    // Respond to the client
    server.send(200, "text/html", "<html><body><h1>Settings Updated!</h1><p><a href='/'>Go back</a></p></body></html>");
}

void setup() {
  Serial.begin(115200);

  // Initialize EEPROM and load the saved LED state
  EEPROM.begin(512);          // Initialize EEPROM with a size of 512 bytes
  enableLED = loadLEDState(); // Load the saved LED state
  strip_brightness = loadBrightness(); // Load the saved brightness
  getFreeEepromSpace();
  getMaxEepromSpace();

  // Set up the onboard LED pin
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, enableLED ? LOW : HIGH);  // Set the LED based on the loaded state


  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Define URL endpoints
  server.on("/", handleRoot);             // Serve the landing page
  server.on("/test", handleRoot2);             // Serve the landing page
  server.on("/submit", HTTP_POST, handleFormSubmit);  // Handle form submissions
  server.on("/update", HTTP_POST, handleFormUpdate);  // Handle form submissions
  server.onNotFound([]() {
    server.send(404, "text/plain", "404: Not found");
  });
  
  // Start the server
  server.begin();
  Serial.println("HTTP server started");

  strip.begin();
  strip.setBrightness(strip_brightness); // Set the brightness from 0 to 255
  // strip.setBrightness(); // Set the brightness from 0 to 255
  strip.setPixelColor(1, COLOR_BLUE);
  strip.show();
}

void loop() {
  server.handleClient();  // Handle incoming client requests
}