// network.h

#ifndef NETWROK_H
#define NETWROK_H


String get_ip_address() {
  return WiFi.localIP().toString();
}

String get_mac_address() {
  return WiFi.macAddress();
}

String get_ssid() {
  return WiFi.SSID();
}

String get_hostname() {
  return WiFi.hostname();
}



#endif