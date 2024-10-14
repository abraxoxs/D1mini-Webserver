// landing_page.h

#ifndef SUBMIT_H
#define SUBMIT_H

#include <Arduino.h> // Ensure that Arduino-specific types like `String` are available


// Function to return the HTML content for the configuration saved page
String getConfigurationSavedPage() {
  String html = "<!DOCTYPE html>";
  html += "<html lang='en'><head><meta charset='UTF-8'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<meta http-equiv='refresh' content='2;url=/'>";
  html += "<style>";
  html += "body { font-family: Arial, sans-serif; margin: 0; padding: 20px; background-color: #f4f4f9; color: #333; text-align: center; }";
  html += "h2 { color: #007BFF; }";
  html += "a { color: #007BFF; text-decoration: none; }";
  html += "a:hover { text-decoration: underline; }";
  html += "</style>";
  html += "</head><body>";
  
  html += "<h2>Configuration Saved!</h2>";
  html += "<a href='/'>Go back to the configuration page</a>";
  html += "</body></html>";

  return html;
}

#endif