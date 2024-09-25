// Created by tludwig on 25.09.24.

#include <Arduino.h>

class LedHandler {
    bool LedBlink = false;

public:
    void init() {
        Serial.println("[LED] Init LED ");
        pinMode(LED_BUILTIN, OUTPUT);
        turnOn();
    }

    void turnOn() {
        digitalWrite(LED_BUILTIN, HIGH);
    }

    void turnOff() {
        digitalWrite(LED_BUILTIN, LOW);
    }

    void blink() {
        digitalWrite(LED_BUILTIN, LOW);
        sleep_ms(250);
        digitalWrite(LED_BUILTIN, HIGH);
    }

    void blinkFast() {
        int iterations = 6;

        for (int i = 0; i < iterations; i++) {
            blink();
            sleep_ms(250);
        }
    }

    void turnBlinkOn() {
        if(LedBlink) {
            return;
        }
        LedBlink = true;
        turnOff();
    }

    void turnBlinkOff() {
        if(!LedBlink) {
            return;
        }
        LedBlink = false;
        turnOn();
    }

    void handleBlink() {
        if(!LedBlink) {
            return;
        }
        blink();
        sleep_ms(1000);
    }
};
