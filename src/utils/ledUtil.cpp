#include "ledUtil.h"
#include <Arduino.h>

bool LedBlink = false;

void LedHandler::init() {
    Serial.println("[LED] Init LED ");
    pinMode(LED_BUILTIN, OUTPUT);
    turnOn();
}

void LedHandler::turnOn() {
    digitalWrite(LED_BUILTIN, HIGH);
}

void LedHandler::turnOff() {
    digitalWrite(LED_BUILTIN, LOW);
}

void LedHandler::blink() {
    digitalWrite(LED_BUILTIN, LOW);
    sleep_ms(250);
    digitalWrite(LED_BUILTIN, HIGH);
}

void LedHandler::blinkFast() {
    int iterations = 6;
    for (int i = 0; i < iterations; i++) {
        blink();
        sleep_ms(250);
    }
}

void LedHandler::turnBlinkOn() {
    if (LedBlink) return;
    LedBlink = true;
    turnOff();
}

void LedHandler::turnBlinkOff() {
    if (!LedBlink) return;
    LedBlink = false;
    turnOn();
}

void LedHandler::handleBlink() {
    if (!LedBlink) return;
    blink();
    sleep_ms(1000);
}
