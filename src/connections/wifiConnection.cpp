#include "wifiConnection.h"

#include "credentials.cpp"

#include <WiFi.h>
#include <Arduino.h>


bool WifiConnection::isConnected()
{
    return WiFi.status() == WL_CONNECTED;
}


void WifiConnection::connect()
{
    String fv = WiFi.firmwareVersion();
    Serial.println("[WN] FirmwareVersion: ");
    Serial.println(fv);

    listNetworks();
    WiFi.mode(WIFI_STA);
    WiFi.begin(Credentials::wifi_ssid, Credentials::wifi_password);

    Serial.print("[WN] Connecting with ");
    Serial.print(Credentials::wifi_ssid);

    int currentTry = 0;

    while (!isConnected()) {
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
    Serial.println("[WN] WiFi Connection with ");
    Serial.println(WiFi.SSID());

    Serial.print("[WN] Assigned IP: ");
    Serial.println(WiFi.localIP());
}


String WifiConnection::getIp()
{
    if (isConnected()) {
        return WiFi.localIP().toString();
    }

    return "No IP";
}


void WifiConnection::listNetworks()
{
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
        printEncryption(WiFi.encryptionType(thisNet));
    }
}


void WifiConnection::printEncryption(int encryptionType)
{
    switch (encryptionType) {
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
        default:
            Serial.println("N/A");
    }
}