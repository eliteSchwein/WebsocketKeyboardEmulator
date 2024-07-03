#include <Arduino.h>
#include <WebSocketsClient.h>
#include <iostream>
#include <string>
#include <ArduinoJson.h>
#include <cstdint>

#include "credentials.cpp"
#include "wifi.cpp"
#include "keyboard.cpp"

WiFiHandler wifi;
KeyboardHandler keyboard;
WebSocketsClient webSocket;

void webSocketEvent(WStype_t type, unsigned char * payload, unsigned length) {
	switch(type) {
		case WStype_DISCONNECTED:
			Serial.println("[WS] Disconnected!");
			break;
		case WStype_CONNECTED:
			Serial.print("[WS] Connected to ");
			Serial.print(Credentials::websocket_ip);
			Serial.print(":");
			Serial.print(Credentials::websocket_port);
			Serial.println("");
			break;
		case WStype_TEXT:
            const char* jsonPayload = (const char*)payload;

            JsonDocument doc;
			deserializeJson(doc, jsonPayload);

            const String method = doc["method"];
            const char* name = doc["data"]["name"];
            const int duration = doc["data"]["duration"];

			if(method != "trigger_keyboard") {
				break;
			}

			if(strcmp(name, Credentials::name) != 0) {
				break;
			}

			JsonArray keys = doc["data"]["keys"];
			
			keyboard.triggerKeys(keys, duration);

			break;
	}

}

void setup() {
    Serial.begin(115200);

    Serial.println("");
    Serial.println("[SYS] Startup WebsocketKeyboardEmulator 0.0.1...");

    wifi.wifiSetup(Credentials::wifi_ssid, Credentials::wifi_password);
    
    keyboard.keyboardSetup();

	webSocket.onEvent(webSocketEvent);

    Serial.println("");
	Serial.print("[WS] connect websocket ");
	Serial.print(Credentials::websocket_ip);
	Serial.print(":");
	Serial.print(Credentials::websocket_port);
    Serial.println("");

    webSocket.begin(Credentials::websocket_ip, Credentials::websocket_port);

	webSocket.setReconnectInterval(1000);
}

void loop() {
	webSocket.loop();
}