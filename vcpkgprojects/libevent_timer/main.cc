#include <event2/event.h>
#include <iostream>

#ifdef _WIN32
#pragma comment(lib, "Ws2_32.lib")
#endif

namespace {
    struct OnApp {
        OnApp() {
#ifdef WIN32
            // Initialize Winsock 2.2
            WSADATA wsaData;
            int err = WSAStartup(MAKEWORD(2, 2), &wsaData);

            if (err) {
                std::cout << "WSAStartup() failed with error: %d" << err;
            }
#endif
        }
        ~OnApp() {
#ifdef WIN32
            system("pause");
            WSACleanup();
#endif
        }
    } __s_onexit_pause;
}


int main() {
    std::cout << "starting ...\n";
    struct event_base* base = event_base_new();
    event_base_dispatch(base);
    event_base_free(base);
    std::cout << "exiting ...\n";
    return 0;
}