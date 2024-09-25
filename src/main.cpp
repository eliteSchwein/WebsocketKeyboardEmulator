#include <Arduino.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>

#include "credentials.cpp"
#include "wifi.cpp"
#include "keyboard.cpp"
#include "led.cpp"

WiFiHandler wifi;
KeyboardHandler keyboard;
WebSocketsClient webSocket;
LedHandler led;

void webSocketEvent(WStype_t type, unsigned char * payload, unsigned length) {
	switch(type) {
		case WStype_DISCONNECTED:
			Serial.println("[WS] Disconnected!");
			led.turnOff();
			break;
		case WStype_CONNECTED:
			Serial.print("[WS] Connected to ");
			Serial.print(Credentials::websocket_ip);
			Serial.print(":");
			Serial.print(Credentials::websocket_port);
			Serial.println("");
			led.turnOn();
			break;
		case WStype_TEXT:
            const char* jsonPayload = (const char*)payload;

            JsonDocument doc;
			deserializeJson(doc, jsonPayload);

            const String method = doc["method"];
            const char* name = doc["data"]["name"];
            const int duration = doc["data"]["duration"];

            if(method == "ping_keyboard") {
            	led.blink();
				webSocket.sendTXT("{\"keyboard_status\": \"online\"}");
              	break;
            }

			if(method != "trigger_keyboard") {
				break;
			}

			if(strcmp(name, Credentials::name) != 0) {
				break;
			}

			led.blink();

			JsonArray keys = doc["data"]["keys"];

			keyboard.triggerKeys(keys, duration);
			led.blink();
			webSocket.sendTXT("{\"keyboard_status\": \"finished\"}");

			break;
	}
}

void setup() {
    Serial.begin(115200);

    Serial.println("");
	Serial.println("[SYS] Startup WebsocketKeyboardEmulator 0.0.2...");

	led.init();
	multicore_reset_core1();
	multicore_launch_core1(loop1);

	led.turnBlinkOn();

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

	led.turnBlinkOff();
}

void loop() {
	webSocket.loop();
}

void loop1() {
	while(true) {
		led.handleBlink();
		if (BOOTSEL) {
			Serial.println("[SYS] Bootsel pressed!");
			Serial.println("[SYS] Hold 5 seconds for flash mode, release for just reboot!");
			Serial.println("[SYS] Reboot in 3 seconds!");
			sleep_ms(3000);
			rp2040.reboot();
		}
        sleep_ms(250);
	}
}