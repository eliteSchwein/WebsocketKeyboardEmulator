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
			Serial.println("");
			break;
		case WStype_CONNECTED:
			Serial.printf("[WSc] Connected to url: %s\n", payload);
			Serial.println("");
			break;
		case WStype_TEXT:
			Serial.printf("[WSc] get text: %s\n", payload);
			Serial.println("");

            const char* jsonPayload = (const char*)payload;

            JsonDocument doc;
			deserializeJson(doc, jsonPayload);

            const String method = doc["method"];
            const char* name = doc["data"]["name"];

			if(method != "trigger_keyboard") {
				break;
			}

			if(strcmp(name, Credentials::name) != 0) {
				break;
			}

			JsonArray keys = doc["data"]["keys"];

			for (JsonVariant keyValue : keys) {
				const char* key = keyValue.as<const char*>(); // Get key as string

				// Simulate key presses based on the key value
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
				} else {
					Keyboard.press(key[0]); // Press the first character of key
				}
			}

			delay(100);

			Keyboard.releaseAll();

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

    Serial.println("");
	Serial.print("connect websocket ");
	Serial.print(Credentials::websocket_ip);
	Serial.print(":");
	Serial.print(Credentials::websocket_port);
    Serial.println("");

    // Connect Websocket
    webSocket.begin(Credentials::websocket_ip, Credentials::websocket_port);

	// try ever 5000 again if connection has failed
	webSocket.setReconnectInterval(5000);
}

void loop() {
	webSocket.loop();
}