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

			const int requestId = doc["id"];

            const String method = doc["method"];
            const char* name = doc["params"]["name"];
            const int duration = doc["params"]["duration"];

            if(method == "ping_keyboard") {
            	led.blink();
				webSocket.sendTXT("{\"jsonrpc\": \"2.0\", \"params\": {\"keyboard_status\": \"online\"}, \"id\": " + String(requestId) + "}");
              	break;
            }

			if(method != "trigger_keyboard" && method != "notify_trigger_keyboard") {
				break;
			}

			if(strcmp(name, Credentials::name) != 0) {
				break;
			}

			led.blink();

			JsonArray keys = doc["params"]["keys"];

			keyboard.triggerKeys(keys, duration);
			led.blink();
			webSocket.sendTXT("{\"jsonrpc\": \"2.0\", \"params\": {\"keyboard_status\": \"finished\"}, \"id\": " + String(requestId) + "}");

			break;
	}
}

void setup() {
    Serial.begin(115200);

    Serial.println("");
	Serial.println("[SYS] Startup WebsocketKeyboardEmulator 0.0.3...");

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
			Serial.println("[SYS] Hold till led off for flash mode, release for just reboot!");
			Serial.println("[SYS] Reboot in 3 seconds!");
			led.blinkFast();
			led.turnOff();
			rp2040.reboot();
		}
        sleep_ms(250);
	}
}