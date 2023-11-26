#pragma once
#include <WinSock2.h>
#include "UDPSocket.h"


namespace sock {
    class ServerUtils {
    public:
        static UDPSocketPtr CreateUDPSocket();
        static bool StaticInit();
        static void CleanUp();

    };
}