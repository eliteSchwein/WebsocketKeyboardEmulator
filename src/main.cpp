#include <Arduino.h>
#include <WebSocketsClient.h>
#include <iostream>
#include <string>
#include <ArduinoJson.h>
#include <cstdint>

#include "credentials.cpp"
#include "wifi.cpp"
#include "Keyboard.h"

WiFiHandler wifi;
WebSocketsClient webSocket;

void webSocketEvent(WStype_t type, unsigned char * payload, unsigned length) {
	switch(type) {
		case WStype_DISCONNECTED:
			Serial.printf("[WSc] Disconnected!\n");
			break;
		case WStype_CONNECTED:
			Serial.printf("[WSc] Connected to url: %s\n", payload);

			// send message to server when Connectedc
			webSocket.sendTXT("Connected");
			break;
		case WStype_TEXT:
			Serial.printf("[WSc] get text: %s\n", payload);

            const char* jsonPayload = (const char*)payload;

            JsonDocument doc;
			deserializeJson(doc, jsonPayload);

            const char* sensor = doc["sensor"];
            long time = doc["time"];
            double latitude = doc["data"][0];
            double longitude = doc["data"][1];

            Serial.println(sensor);
            Serial.println(time);
            Serial.println(latitude, 6);
            Serial.println(longitude, 6);

			// send message to server
			// webSocket.sendTXT("message here");
			break;
	}

}

void setup() {
    Serial.begin(115200);

    while (!Serial) {
        delay(10); // hang out until serial port opens
    }

    Serial.println("");
    Serial.println("Setting Up Pico...");

    // Start wifi connection
    wifi.wifiSetup(Credentials::wifi_ssid, Credentials::wifi_password);
    
    // Start Keyboard Emulator
    Keyboard.begin();

	// event handler
	webSocket.onEvent(webSocketEvent);

    // Connect Websocket
    webSocket.begin(Credentials::websocket_ip, Credentials::websocket_port, "/");

	// try ever 5000 again if connection has failed
	webSocket.setReconnectInterval(5000);
}

void loop() {
	webSocket.loop();
}