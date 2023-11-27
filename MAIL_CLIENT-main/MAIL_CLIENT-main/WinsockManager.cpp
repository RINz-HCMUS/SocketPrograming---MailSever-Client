#include "WinsockManager.h"
#include <iostream>

WinsockManager::WinsockManager() {
    // Create Windows sockets API
    WORD w_version_requested = MAKEWORD(2, 2); // version 2.2 of Winsock
    // WSAStarup return 0 on success 
    if (WSAStartup(w_version_requested, &wsa_data) != 0)
    {
        std::cerr << "Failed to initialize Winsock." << std::endl;
    }
}

WinsockManager::~WinsockManager() {
    WSACleanup();
}

WinsockManager& WinsockManager::getInstance() {
    static WinsockManager instance;
    return instance;
}
