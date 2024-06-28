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
			Serial.println("[WS] Disconnected!");
			break;
		case WStype_CONNECTED:
			Serial.printf("[WS] Connected to url: %s\n", payload);
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

			for (JsonVariant keyValue : keys) {
				const char* key = keyValue.as<const char*>();

				Serial.print("[KB] press key ");
				Serial.print(key[0]);
				Serial.println("");
				
				if (strcmp(key, "ctrl_left") == 0) {
					Keyboard.press(KEY_LEFT_CTRL);
				} else if (strcmp(key, "ctrl_right") == 0) {
					Keyboard.press(KEY_RIGHT_CTRL);
				} else if (strcmp(key, "alt_left") == 0) {
					Keyboard.press(KEY_LEFT_ALT);
				} else if (strcmp(key, "alt_right") == 0) {
					Keyboard.press(KEY_RIGHT_ALT);
				} else if (strcmp(key, "space") == 0) {
					Keyboard.press(' ');
				} else if (strcmp(key, "backspace") == 0) {
					Keyboard.press(KEY_BACKSPACE);
				} else if (strcmp(key, "insert") == 0) {
					Keyboard.press(KEY_INSERT);
				} else if (strcmp(key, "delete") == 0) {
					Keyboard.press(KEY_DELETE);
				} else if (strcmp(key, "esc") == 0) {
					Keyboard.press(KEY_ESC);
				} else if (strcmp(key, "tab") == 0) {
					Keyboard.press(KEY_TAB);
				} else if (strcmp(key, "home") == 0) {
					Keyboard.press(KEY_HOME);
				} else if (strcmp(key, "end") == 0) {
					Keyboard.press(KEY_END);
				} else {
					Keyboard.press(key[0]);
				}
			}

			if(duration > 0) {
				delay(duration);
			} else {
				delay(100);
			}

			Serial.print("[KB] release Keys");
			Serial.println("");

			Keyboard.releaseAll();

			break;
	}

}

void setup() {
    Serial.begin(115200);

    Serial.println("");
    Serial.println("[SYS] Startup WebsocketKeyboardEmulator 0.0.1...");

    wifi.wifiSetup(Credentials::wifi_ssid, Credentials::wifi_password);
    
    Keyboard.begin();

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