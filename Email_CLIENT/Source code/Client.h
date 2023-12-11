#include <iostream>
#include <string>
#include <winsock2.h>
#include "base64.h"

#pragma comment(lib, "ws2_32.lib")

#ifndef WINSOCK_MANAGER_H
#define WINSOCK_MANAGER_H


class WinsockManager {
private:
    WSADATA wsa_data;

    WinsockManager() 
    {
        // Create Windows sockets API
        WORD w_version_requested = MAKEWORD(2, 2); // version 2.2 of Winsock
        // WSAStarup return 0 on success 
        if (WSAStartup(w_version_requested, &wsa_data) != 0)
        {
            std::cerr << "Failed to initialize Winsock." << std::endl;
        }
    }
    ~WinsockManager() 
    {
        WSACleanup();
    }

public:
    // use to initialize WSA once time through application
    static WinsockManager& getInstance() 
    {
        static WinsockManager instance;
        return instance;
    }
};

#endif // WINSOCK_MANAGER_H

#ifndef CLIENT_H
#define CLIENT_H

class Client
{
protected:
    std::string address;
    int port;
    SOCKET client_socket;

    // send command to server
    void sendCommand(const std::string& command) const;

    // receive response from server
    std::string receiveResponse() const;

    // display response from the server - use for debug
    void displayResponse(const std::string& response) const;

public:

    Client(const std::string& server_address, int server_port);
    virtual ~Client();

    // connect to the server
    bool connectToServer() ;
};


#endif // CLIENT_H