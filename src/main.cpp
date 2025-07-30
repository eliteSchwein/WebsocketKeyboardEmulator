#include <Arduino.h>
#include <WebSocketsClient.h>

#include "utils/ledUtil.h"
#include "utils/keyboardUtil.h"

#include "connections/wifiConnection.h"
#include "connections/websocketConnection.h"

WebsocketConnection websocket;
WifiConnection wifi;

KeyboardUtil keyboard;
LedHandler led;

void setup() {
    Serial.begin(115200);

    Serial.println("");
	Serial.println("[SYS] Startup WebsocketKeyboardEmulator 0.0.4...");

	led.init();
	multicore_reset_core1();
	multicore_launch_core1(loop1);

	led.turnBlinkOn();

    wifi.connect();

	keyboard.setup();

	websocket.connect();

	led.turnBlinkOff();
}

void loop() {
	websocket.loop();
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