#include <WiFi.h>

class WiFiHandler {
public:
    // Method to check if WiFi is connected
    bool isWifiConnected() {
        return WiFi.status() == WL_CONNECTED;
    }

    // Method to set up WiFi connection
    void wifiSetup(const char *ssid, const char *password) {
        WiFi.mode(WIFI_STA); // Set WiFi mode to station
        WiFi.begin(ssid, password); // Begin WiFi connection

        Serial.print("Connecting");
        while (!isWifiConnected()) {
            delay(500);
            Serial.print(".");
        }

        // Connection established
        Serial.println();
        Serial.println("WiFi Connection Established!!");
        Serial.print("SSID: ");
        Serial.println(WiFi.SSID());

        // Print IP Address
        Serial.print("Assigned IP: ");
        Serial.println(WiFi.localIP());
    }

    // Method to get the IP address as a string
    String getIpAddrToPrint() {
        if (isWifiConnected()) {
            return WiFi.localIP().toString();
        } else {
            return "No IP";
        }
    }
};