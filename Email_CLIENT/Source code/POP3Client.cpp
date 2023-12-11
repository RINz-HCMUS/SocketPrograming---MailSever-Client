#include "POP3Client.h"

std::string POP3Client::receiveResponsePOP3() const
{
    char buffer[1024];
    std::string response;

    while (true)
    {
        int bytes = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        if (bytes > 0)
        {
            buffer[bytes] = '\0';
            response += buffer;

            size_t pos = response.find("\r\n.\r\n");
            if (pos != std::string::npos)
            {
                response.resize(pos + 5);
                break;
            }
        }
        else
        {
            break;
        }
    }

    // displayResponse(response);

    return response;
}


bool POP3Client::login(const std::string& username, const std::string& password)
{
    // Connect to the server
    if (!connectToServer())
    {
        std::cerr << "Failed to connect to the server.\n";
        return false;
    }

    // Send the USER command
    sendCommand("USER " + username + "\r\n");
    std::string userResponse = receiveResponse();  // Receive and store the response
    if (userResponse.substr(0, 3) != "+OK")
    {
        std::cerr << "Failed to send USER command\n";
        return false;
    }

    // Send the PASS command
    sendCommand("PASS " + password + "\r\n");
    std::string passResponse = receiveResponse();  // Receive and store the response
    if (passResponse.substr(0, 3) != "+OK")
    {
        std::cerr << "Failed to login\n";
        return false;
    }

    return true;
}

void POP3Client::getCountEmail() 
{
    // assume that it is being connected to the server 
    sendCommand("STAT\r\n"); 

    std::string stat_response = receiveResponse();
    std::istringstream iss(stat_response);
    std::string status;
    int count_email = 0;
    iss >> status >> count_email;
    this->count_email = count_email;
}

void POP3Client::getListUID()
{
    // assume that it is being connected to the server 
    sendCommand("UIDL\r\n");

    std::string uidl_response = receiveResponsePOP3();

    std::istringstream iss(uidl_response);
    std::string status;
    

    list_uid.clear();
    list_uid.push_back(""); // "" for [0]

    iss >> status;
    if (status == "+OK")
    for (int i = 0; i < count_email; ++i)
    {
        int email_number;
        std::string email_UID;
        iss >> email_number >> email_UID;
        list_uid.push_back(email_UID);
    }
}

std::string POP3Client::retrieveEmail(int email_number)
{
    // assume that it is being connected to the server 
    
    // check if this email had downloaded 
    std::string email_uid = list_uid[email_number];
    if (downloaded_email.find(email_uid) != downloaded_email.end())
    {
        //debug
        // std::cout << "Email with UID " << email_uid << " has already been downloaded.\n";
        return "";
    }


    sendCommand("RETR " + std::to_string(email_number) + "\r\n");

    std::string email_content = receiveResponsePOP3();

    downloaded_email.insert(email_uid);

    return email_content;
}

std::vector<std::pair<std::string, std::string>> POP3Client::retrieveAllEmail()
{
    getCountEmail();
    getListUID();
    std::vector<std::pair<std::string, std::string>> list_email;
    for(int i = 1; i <= count_email; i++)
    {
        std::string email_name = list_uid[i].substr(0, list_uid[i].find(".msg"));
        std::string email_content = retrieveEmail(i);
        if (email_content != "")
            list_email.push_back({email_content, email_name});
    }
    sendCommand("QUIT\r\n");
    return list_email;
}

void POP3Client::loadDownloadedEmail(const std::string& username)
{
    downloaded_email.clear();
    std::string database_folder = "database";
    std::string filename = "downloaded_email_UID.txt";
    std::string file_path = database_folder + "/" + username + "/" + filename;

    std::ifstream file(file_path);
    if (file.is_open()) 
    {
        std::string email;
        while (std::getline(file, email)) {
            downloaded_email.insert(email);
        }
        file.close();
    }
}

void POP3Client::saveDownloadEmail(const std::string& username)
{
    std::string database_folder = "database";
    std::string filename = "downloaded_email_UID.txt";
    std::string user_folder = database_folder + "/" + username;
    std::string file_path = user_folder + "/" + filename;

    // Create the folder if it doesn't exist
    if (!std::filesystem::exists(database_folder)) 
    {
        std::filesystem::create_directory(database_folder);
    }

    if (!std::filesystem::exists(user_folder)) 
    {
        std::filesystem::create_directory(user_folder);
    }

    std::ofstream file(file_path);
    if (file.is_open()) 
    {
        for (const auto& email : downloaded_email) 
        {
            file << email << "\n";
        }
        file.close();
    }
}
