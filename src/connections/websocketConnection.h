#pragma once

#include <WebSocketsClient.h>

class WebsocketConnection
{
private:
    static void event(WStype_t type, unsigned char * payload, unsigned length);

public:
    void connect();
    void loop();
};