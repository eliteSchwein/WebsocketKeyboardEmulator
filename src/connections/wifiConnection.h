#pragma once

#include <Arduino.h>

class WifiConnection
{
public:
    bool isConnected();
    void connect();
    String getIp();
    void listNetworks();
    void printEncryption(int encryptionType);
};
