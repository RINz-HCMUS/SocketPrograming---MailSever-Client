#include <iostream>
#include <string>
#include <winsock2.h>
#include "base64.h"
#include "WinsockManager.h"

#pragma comment(lib, "ws2_32.lib")

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