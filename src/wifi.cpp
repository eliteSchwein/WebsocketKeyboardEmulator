#include <WiFi.h>

class WiFiHandler {
public:
    // Method to check if WiFi is connected
    bool isWifiConnected() {
        return WiFi.status() == WL_CONNECTED;
    }

    // Method to set up WiFi connection
    void wifiSetup(const char *ssid, const char *password) {
        String fv = WiFi.firmwareVersion();
        Serial.println("FirmwareVersion: ");
        Serial.println(fv);

        listNetworks();
        WiFi.mode(WIFI_STA); // Set WiFi mode to station
        WiFi.begin(ssid, password); // Begin WiFi connection

        Serial.print("Connecting with ");
        Serial.print(ssid);

        int currentTry = 0;

        while (!isWifiConnected()) {
            if(currentTry == 11) {
                Serial.println(" ");
                listNetworks();
                currentTry = 0;
            }
            delay(500);
            Serial.print(".");
            currentTry++;
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

    void listNetworks() {
        Serial.println("Scanning available networks...");
        // scan for nearby networks:
        Serial.println("** Scan Networks **");
        int numSsid = WiFi.scanNetworks();
        if (numSsid == -1) {
            Serial.println("Couldn't get a wifi connection");
            while (true);
        }

        // print the list of networks seen:
        Serial.print("number of available networks:");
        Serial.println(numSsid);

        // print the network number and name for each network found:
        for (int thisNet = 0; thisNet < numSsid; thisNet++) {
            Serial.print(thisNet);
            Serial.print(") ");
            Serial.print(WiFi.SSID(thisNet));
            Serial.print("\tSignal: ");
            Serial.print(WiFi.RSSI(thisNet));
            Serial.print(" dBm");
            Serial.print("\tEncryption: ");
            printEncryptionType(WiFi.encryptionType(thisNet));
        }
    }

    void printEncryptionType(int thisType) {
        // read the encryption type and print out the name:
        switch (thisType) {
            case ENC_TYPE_WEP:
            Serial.println("WEP");
            break;
            case ENC_TYPE_TKIP:
            Serial.println("WPA");
            break;
            case ENC_TYPE_CCMP:
            Serial.println("WPA2");
            break;
            case ENC_TYPE_NONE:
            Serial.println("None");
            break;
            case ENC_TYPE_AUTO:
            Serial.println("Auto");
            break;
        }
    }
};