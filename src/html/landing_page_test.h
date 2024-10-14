// landing_page_test.h

#ifndef LANDING_PAGE_TEST_H
#define LANDING_PAGE_TEST_H

#include <Arduino.h> // Ensure that Arduino-specific types like `String` are available

// Function to return the HTML content for the landing page
String getLandingPageTest(bool enableLED, bool enableLogging, bool enableAlerts,
                      String ipAddress, String macAddress, String ssid, String hostname,
                      int eeprom_space, int eeprom_max_space, String free_heap, int strip_brightness) {
    String html = "<!DOCTYPE html>";
    html += "<html lang='en'><head><meta charset='UTF-8'>";
    html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
    
    // Inline CSS for styling the switches and other elements
    html += "<style>";
    html += "body { font-family: Arial, sans-serif; margin: 0; padding: 20px; background-color: #f4f4f9; color: #333; }";
    html += "h1 { text-align: center; color: #007BFF; }";
    html += ".switch { position: relative; display: inline-block; width: 40px; height: 20px; }"; // Reduced width and height
    html += ".switch input { opacity: 0; width: 0; height: 0; }";
    html += ".slider { position: absolute; cursor: pointer; top: 0; left: 0; right: 0; bottom: 0; background-color: #ccc; transition: .4s; border-radius: 20px; }"; // Adjusted border-radius
    html += ".slider:before { position: absolute; content: ''; height: 16px; width: 16px; left: 2px; bottom: 2px; background-color: white; transition: .4s; border-radius: 50%; }"; // Adjusted dimensions
    html += "input:checked + .slider { background-color: #007BFF; }";
    html += "input:checked + .slider:before { transform: translateX(20px); }"; // Adjusted translate value for the smaller switch
    html += ".info-box { max-width: 600px; margin: 20px auto; background: #fff; padding: 20px; border-radius: 10px; box-shadow: 0 2px 10px rgba(0, 0, 0, 0.1); }";
    html += "</style>";

    // JavaScript for handling onchange events and sending them to the server
    html += "<script>";
    html += "function updateConfig(setting, value) {";
    html += "  fetch('/update', {";
    html += "    method: 'POST',";
    html += "    headers: { 'Content-Type': 'application/x-www-form-urlencoded' },";
    html += "    body: setting + '=' + value";
    html += "  }).then(response => response.text()).then(data => { console.log(data); });";
    html += "}";
    html += "</script>";

    html += "</head><body>";

    html += "<h1>Device Configuration</h1>";
    html += "<div class='info-box'>";
    html += "<table style='width: 100%; border-collapse: collapse;'>";

    // Row for Enable LED switch
    html += "<tr>";
    html += "<td style='padding: 8px;'>Enable LED</td>";
    html += "<td style='padding: 8px; text-align: right;'>";
    html += "<label class='switch'>";
    html += "<input type='checkbox' id='enableLED' name='enableLED' onchange='updateConfig(\"enableLED\", this.checked)' " + String(enableLED ? "checked" : "") + ">";
    html += "<span class='slider'></span>";
    html += "</label>";
    html += "</td>";
    html += "</tr>";

    // Row for Enable Logging switch
    html += "<tr>";
    html += "<td style='padding: 8px;'>Enable Logging</td>";
    html += "<td style='padding: 8px; text-align: right;'>";
    html += "<label class='switch'>";
    html += "<input type='checkbox' id='enableLogging' name='enableLogging' onchange='updateConfig(\"enableLogging\", this.checked)' " + String(enableLogging ? "checked" : "") + ">";
    html += "<span class='slider'></span>";
    html += "</label>";
    html += "</td>";
    html += "</tr>";

    // Row for Enable Alerts switch
    html += "<tr>";
    html += "<td style='padding: 8px;'>Enable Alerts</td>";
    html += "<td style='padding: 8px; text-align: right;'>";
    html += "<label class='switch'>";
    html += "<input type='checkbox' id='enableAlerts' name='enableAlerts' onchange='updateConfig(\"enableAlerts\", this.checked)' " + String(enableAlerts ? "checked" : "") + ">";
    html += "<span class='slider'></span>";
    html += "</label>";
    html += "</td>";
    html += "</tr>";

    // Row for Brightness slider
    html += "<tr>";
    html += "<td style='padding: 8px;'>Brightness</td>";
    html += "<td style='padding: 8px; text-align: right;'>";
    html += "<input type='range' id='brightness' name='brightness' min='1' max='255' value='" + String(strip_brightness) + "' oninput='this.nextElementSibling.value = this.value' onchange='updateConfig(\"brightness\", this.value)'>";
    html += "<output>" + String(strip_brightness) + "</output>";  // Displays the current strip_brightness value
    html += "</td>";
    html += "</tr>";

    html += "</table>";
    html += "</div>";

    // Device Information box
    html += "<div class='info-box'>";
    html += "<h2 style='text-align: left;'>Device Information</h2>";
    html += "<table style='width: 100%; border-collapse: collapse;'>";
    html += "<tr><td style='padding: 8px;'><strong>IP:</strong></td><td style='padding: 8px;'>" + ipAddress + "</td></tr>";
    html += "<tr><td style='padding: 8px;'><strong>MAC Address:</strong></td><td style='padding: 8px;'>" + macAddress + "</td></tr>";
    html += "<tr><td style='padding: 8px;'><strong>SSID:</strong></td><td style='padding: 8px;'>" + ssid + "</td></tr>";
    html += "<tr><td style='padding: 8px;'><strong>Hostname:</strong></td><td style='padding: 8px;'>" + hostname + "</td></tr>";
    html += "<tr><td style='padding: 8px;'><strong>EEPROM Max:</strong></td><td style='padding: 8px;'>" + String(eeprom_max_space) + "</td></tr>";
    html += "<tr><td style='padding: 8px;'><strong>EEPROM Used:</strong></td><td style='padding: 8px;'>" + String(eeprom_space) + "</td></tr>";
    html += "<tr><td style='padding: 8px;'><strong>Space:</strong></td><td style='padding: 8px;'>" + free_heap + "</td></tr>";
    html += "</table>";
    html += "</div>";

    html += "</body></html>";

    return html;
}

#endif
