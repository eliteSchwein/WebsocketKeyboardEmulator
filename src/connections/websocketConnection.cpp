#include "websocketConnection.h"

#include <ArduinoJson.h>

#include "credentials.cpp"
#include "utils/keyboardUtil.h"

#include "utils/ledUtil.h"

WebSocketsClient webSocket;

extern LedHandler led;
extern KeyboardUtil keyboard;

void WebsocketConnection::event(WStype_t type, unsigned char* payload, unsigned length)
{
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

    case WStype_TEXT: {
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

    default:
        Serial.println("[WS] Unregistered Event!");
    }
}


void WebsocketConnection::connect()
{
    webSocket.onEvent(event);

    Serial.println("");
    Serial.print("[WS] connect websocket ");
    Serial.print(Credentials::websocket_ip);
    Serial.print(":");
    Serial.print(Credentials::websocket_port);
    Serial.println("");

    webSocket.begin(Credentials::websocket_ip, Credentials::websocket_port);

    webSocket.setReconnectInterval(1000);

    webSocket.enableHeartbeat(15000, 3000, 2);
}

void WebsocketConnection::loop()
{
    webSocket.loop();
}
