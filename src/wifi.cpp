#include <WiFi.h>

class WiFiHandler {
public:
    bool isWifiConnected() {
        return WiFi.status() == WL_CONNECTED;
    }

    void wifiSetup(const char *ssid, const char *password) {
        String fv = WiFi.firmwareVersion();
        Serial.println("[WN] FirmwareVersion: ");
        Serial.println(fv);

        listNetworks();
        WiFi.mode(WIFI_STA);
        WiFi.begin(ssid, password);

        Serial.print("[WN] Connecting with ");
        Serial.print(ssid);

        int currentTry = 0;

        while (!isWifiConnected()) {
            if(currentTry == 21) {
                Serial.println(" ");
                listNetworks();
                currentTry = 0;
            }
            delay(500);
            Serial.print(".");
            currentTry++;
        }
        
        Serial.println();
        Serial.println("[WN] WiFi Connection wit h");
        Serial.println(WiFi.SSID());

        Serial.print("[WN] Assigned IP: ");
        Serial.println(WiFi.localIP());
    }

    String getIpAddrToPrint() {
        if (isWifiConnected()) {
            return WiFi.localIP().toString();
        } else {
            return "No IP";
        }
    }

    void listNetworks() {
        Serial.println("[WN] Scanning available networks...");
        
        int numSsid = WiFi.scanNetworks();
        if (numSsid > 1) {
            Serial.println("[WN] Couldn't find networks!");
            return;
        }

        Serial.print("[WN] Scanned Networks:");
        Serial.println(numSsid);

        for (int thisNet = 0; thisNet < numSsid; thisNet++) {
            Serial.print("[WN] ");
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