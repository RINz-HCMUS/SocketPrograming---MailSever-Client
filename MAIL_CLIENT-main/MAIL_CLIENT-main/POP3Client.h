#include "Client.h"
#include <set>
#include <vector>
#include <sstream>
#include "Email.h"
#include <filesystem>


#ifndef POP3_CLIENT_H
#define POP3_CLIENT_H

class POP3Client : public Client
{
private:
    std::set<std::string> downloaded_email;
    std::vector<std::string> list_uid;
    int count_email;

    std::string receiveResponsePOP3() const;

    void getCountEmail();
    void getListUID();

    std::string retrieveEmail(int email_number);

public:
        // constructor POP3Client(address, port)
    POP3Client(const std::string& server_address, int server_port)
        : Client(server_address, server_port){}
    ~POP3Client(){}

        // use to login to the pop3 server,
    bool login(const std::string& username, const std::string& password);
        
        // retrieve all email and assign to list email.
        // return list_email <email_content, email_name> 
    std::vector<std::pair<std::string, std::string>> retrieveAllEmail();

        // load file downloaded_email_uid.txt from database
        // use before retrieve all email
    void loadDownloadedEmail(const std::string& username);
        // save file downloaded_email_uid.txt from database
        // use after retrieve all email
    void saveDownloadEmail(const std::string& username);

};

#endif // POP3_CLIENT_H