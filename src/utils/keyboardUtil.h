#pragma once

#include <map>
#include <string>

#include <ArduinoJson.h>

class KeyboardUtil {
private:
    static const std::map<std::string, int> keymap;

public:
    void setup();
    void triggerKeys(const JsonArray &keys, const int duration);
};
