#include <ArduinoJson.h>
#include <map>

#include "Keyboard.h"

#include "keyboardUtil.h"

const std::map<std::string, int> KeyboardUtil::keymap{
        {"ctrl_left", KEY_LEFT_CTRL},
        {"ctrl_right", KEY_RIGHT_CTRL},
        {"alt_left", KEY_LEFT_ALT},
        {"alt_right", KEY_RIGHT_ALT},
        {"insert", KEY_INSERT},
        {"delete", KEY_DELETE},
        {"esc", KEY_ESC},
        {"tab", KEY_TAB},
        {"home", KEY_HOME},
        {"end", KEY_END},
        {"backspace", KEY_BACKSPACE}
};

void KeyboardUtil::setup()
{
        Serial.print("[KB] start keyboard");
        Keyboard.begin();
}

void KeyboardUtil::triggerKeys(const JsonArray &keys, const int duration)
{
        for (JsonVariant keyValue: keys) {
                const char *key = keyValue.as<const char *>();

                Serial.print("[KB] press key ");
                Serial.print(key);
                Serial.println("");

                auto it = keymap.find(key);
                if (it != keymap.end()) {
                        Keyboard.press(it->second);
                } else if (strcmp(key, "space") == 0) {
                        Keyboard.press(' ');
                } else {
                        Keyboard.press(key[0]);
                }
        }

        if (duration > 0) {
                delay(duration);
        } else {
                delay(100);
        }

        Serial.print("[KB] release Keys");
        Serial.println("");

        Keyboard.releaseAll();
}
