#include <Arduino.h>
#include <WebSocketsClient.h>
#include <iostream>
#include <string>
#include <ArduinoJson.h>

#include "credentials.cpp"
#include "wifi.cpp"
#include "Keyboard.h"

WiFiHandler wifi;
WebSocketsClient webSocket;

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
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

  			StaticJsonDocument<200> doc;

			char json[] = std::to_string(payload);

			// Deserialize the JSON document
			DeserializationError error = deserializeJson(doc, json);

			if (error) {
				Serial.print(F("deserializeJson() failed: "));
				Serial.println(error.f_str());
				return;
			}

			// Fetch values.
			//
			// Most of the time, you can rely on the implicit casts.
			// In other case, you can do doc["time"].as<long>();
			const char* sensor = doc["sensor"];
			long time = doc["time"];
			double latitude = doc["data"][0];
			double longitude = doc["data"][1];

			// Print values.
			Serial.println(sensor);
			Serial.println(time);
			Serial.println(latitude, 6);
			Serial.println(longitude, 6);

			// send message to server
			// webSocket.sendTXT("message here");
			break;
		case WStype_ERROR:			
		case WStype_FRAGMENT_TEXT_START:
		case WStype_FRAGMENT_BIN_START:
		case WStype_FRAGMENT:
		case WStype_FRAGMENT_FIN:
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
	webSocket.setReconnectInterval(5000)
}

void loop() {
	webSocket.loop();
}