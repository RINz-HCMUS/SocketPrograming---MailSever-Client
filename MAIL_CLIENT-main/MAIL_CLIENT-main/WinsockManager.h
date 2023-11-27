#ifndef WINSOCK_MANAGER_H
#define WINSOCK_MANAGER_H

#include <winsock2.h>

class WinsockManager {
private:
    WSADATA wsa_data;

    WinsockManager();
    ~WinsockManager();

public:
    // use to initialize WSA once time through application
    static WinsockManager& getInstance();
};

#endif // WINSOCK_MANAGER_H
