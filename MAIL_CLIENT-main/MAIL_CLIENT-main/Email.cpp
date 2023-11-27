#include "Email.h"

#include <map>
#include <ctime>
#include <iomanip>

const int FILE_SIZE_MAX = 3 * 1024 * 1024;

void DEBUG(std::string line)
{
    std::cout << "\nDEBUG: " << line.size() << " " << line;
}

void Email::addCc(const std::string& recipient)
{
    cc_list.push_back(recipient);
}

void Email::addBcc(const std::string& recipient)
{
    bcc_list.push_back(recipient);
}

void Email::attachFile(const std::string& file_path)
{
    // Check if file is larger than 3MB
    std::ifstream file(file_path, std::ios::binary);
    if (file.is_open())
    {
        file.seekg(0, std::ios::end);
        std::streampos file_size = file.tellg();
        if (file_size > FILE_SIZE_MAX)
        {
            std::cerr << "File :" << file_path << " is larger than 3MB" << std::endl;
        }
        else
        {
            attachment_list.push_back(file_path);
        }
        file.close();
    }
    else
    {
        std::cerr << "Error opening file: " << file_path << std::endl;
    }
    
}

//Getter
std::string Email::getSender() const
{
    return email_sender;
}

std::string Email::getRecipient() const
{
    return email_recipient;
}

std::string Email::getCC() const 
{
    std::string res = "";
    if (!cc_list.empty())
    {
        for (const auto& recipient : cc_list)
            res += recipient + ", ";
        return res.substr(0, res.size() - 2);    
    }
    return res;    
}

std::vector<std::string> Email::getListCC() const 
{
    return cc_list;
}

std::string Email::getBCC() const
{
    std::string res = "";
    if (!bcc_list.empty())
    {
        for (const auto& recipient : bcc_list)
            res += recipient + ", ";
        return res.substr(0, res.size() - 2);    
    }
    return res; 
}

std::vector<std::string> Email::getListBCC() const 
{
    return bcc_list;
}

std::string Email::getSubject() const
{
    return subject;
}

std::string Email::getMessage() const
{
    return message;
}

std::vector<std::pair<std::string, std::string>> Email::getAttachment() const
{
    std::vector<std::pair<std::string, std::string>> file_content_and_filename;
    for (int i = 0; i < attachment_list.size(); ++i)
        file_content_and_filename.push_back({attachment_list[i], attachment_filename_list[i]});
    return file_content_and_filename;
}

std::string Email::getDate() const
{
    return date;
}

// Handle file_path
std::string Email::getFilename(const std::string& file_path) const
{
    return file_path.substr(file_path.find_last_of("/\\") + 1);
}

std::string Email::getFileExtension(const std::string& file_name) const
{
    return file_name.substr(file_name.find_last_of(".") + 1);
}

std::string Email::getContentType(const std::string& file_extension) const 
{
    std::map<std::string, std::string> content_type_map = {
        {"txt", "text/plain"},
        {"jpg", "image/jpeg"},
        {"jpeg", "image/jpeg"},
        {"png", "image/png"},
        {"gif", "image/gif"},
        {"mp4", "video/mp4"},
        {"pdf", "application/pdf"},
        {"html", "text/html"},
        {"css", "text/css"},
        {"js", "application/javascript"},
        {"json", "application/json"},
        {"xml", "application/xml"},
        {"doc", "application/msword"},
        {"docx", "application/vnd.openxmlformats-officedocument.wordprocessingml.document"},
        {"xls", "application/vnd.ms-excel"},
        {"xlsx", "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"},
        {"ppt", "application/vnd.ms-powerpoint"},
        {"pptx", "application/vnd.openxmlformats-officedocument.presentationml.presentation"},
        {"zip", "application/zip"}
    };

    auto it = content_type_map.find(file_extension);
    if (it != content_type_map.end()) {
        return it->second;
    } else {
        return "application/octet-stream";
    }
}


// Handle mail format
std::string Email::getDomain() const
{
    size_t atPos = email_sender.find("@");
    if (atPos != std::string::npos)
    {
        return email_sender.substr(atPos + 1);
    }
    return "";
}


std::string Email::genUniqueString(int size_string) const
{
    const std::string charset = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    int size_charset = charset.size();

    std::ostringstream res;
    for (int i = 0; i < size_string; i++)
        res << charset[rand() % size_charset];
    return res.str();
}

std::string Email::genDate() const
{
    auto current_time = std::time(nullptr);
    current_time += (7 * 3600); // convert to GMT+7

    std::tm* GMT7_tm = std::gmtime(&current_time);

    char buffer[100];
    std::strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S +0700", GMT7_tm);

    return buffer;

}

std::string Email::genUserAgent() const
{
    return "NO NAME";
}

std::string Email::genMessageID() const 
{
    std::ostringstream ID;
    ID << "<" << genUniqueString(20) << "@" << getDomain()  << ">";
    return ID.str();
}

std::string Email::genBoundary() const
{
    std::ostringstream boundary;
    for (int i = 0; i < 12; i++)
        boundary << "-";
    boundary << genUniqueString(24);
    return boundary.str();
}



std::string Email::formatEmail() const {
    std::ostringstream email_format;

    std::string boundary = genBoundary();

    // Check if there are attachments
    bool hasAttachments = !attachment_list.empty();

    // Set the Content-Type header for multipart messages
    if (hasAttachments)
        email_format << "Content-Type: multipart/mixed; boundary=\"" << boundary << "\"\r\n";

    // Common email headers
    email_format << "Message-ID: " << genMessageID() << "\r\n";
    email_format << "Date: " << genDate() << "\r\n";
    email_format << "MIME-Version: 1.0\r\n";
    email_format << "User-Agent: " << genUserAgent() << "\r\n";
    email_format << "Content-Language: en-US\r\n";

    // Sender and recipient information
    email_format << "From: " << email_sender << "\r\n";
    email_format << "To: " << email_recipient << "\r\n";
    email_format << "CC: " << getCC() << "\r\n";
    // Uncomment the line below if BCC is needed
    // email_format << "BCC: " << getBCC() << "\r\n";
    if (hasAttachments)
        email_format << "Subject: " << subject << "\r\n\r\n";
    else
        email_format << "Subject: " << subject << "\r\n";

    // Main message content
    if (hasAttachments) {
        // Boundary for the text/plain part
        email_format << "--" << boundary << "\r\n";
        email_format << "Content-Type: text/plain; charset=utf-8; format=flowed\r\n";
        email_format << "Content-Transfer-Encoding: 7bit\r\n\r\n";
        email_format << message << "\r\n\r\n";
    } else {
        // Single-part body
        email_format << "Content-Type: text/plain; charset=utf-8; format=flowed\r\n";
        email_format << "Content-Transfer-Encoding: 7bit\r\n\r\n";
        email_format << message << "\r\n";
    }

    // Attachments
    for (const auto& file_path : attachment_list) {
        // DEBUG
        // std::cout << "Attempting to open file: " << file_path << std::endl;

        std::string file_name = getFilename(file_path);

        email_format << "--" << boundary << "\r\n";

        // Determine content type based on file extension
        std::string file_extension = getFileExtension(file_name);
        std::string content_type = getContentType(file_extension);

        email_format << "Content-Type: " << content_type << "; name=" << file_name << "\r\n";
        email_format << "Content-Disposition: attachment; filename=" << file_name << "\r\n";
        email_format << "Content-Transfer-Encoding: base64\r\n\r\n";

        // Read and encode file content in base64
        std::ifstream file(file_path, std::ios::binary);
        if (file.is_open()) {
            std::ostringstream file_content;
            file_content << file.rdbuf();
            file.close();

            const int chunk_size = 1024;
            std::string base64_content = base64_encode(reinterpret_cast<const unsigned char*>(file_content.str().data()), file_content.str().size());
            // email_format << base64_content << "\r\n";
            for (size_t i = 0; i < base64_content.size(); i += chunk_size)
            {
                size_t chunk_end = std::min(i + chunk_size, base64_content.size());
                std::string chunk = base64_content.substr(i, chunk_end - i) + "\r\n";
                email_format << chunk;
            }

        } else {
            // Handle file opening error
            std::cerr << "Error opening file: " << file_path << std::endl;
        }
    }

    // End of email
    if (hasAttachments) {
        email_format << "--" << boundary << "--" << "\r\n";
    }

    return email_format.str();
}


std::string eraseWhitespace(std::string str)
{
    str.erase(str.find_last_not_of(" \t\r\n") + 1);
    str.erase(0, str.find_first_not_of(" \t\r\n"));
    return str;
}

std::string eraseQuotationMarks(std::string str)
{
    str.erase(str.find_last_of("\""));
    str.erase(0, str.find_first_not_of("\""));
    return str;
}

Email::Email(const std::string& email_content)
{
    std::istringstream email_stream(email_content);
    std::string line;

    cc_list.clear();
    bcc_list.clear();
    attachment_list.clear();
    attachment_filename_list.clear();
    message.clear();

    size_t boundary_pos = email_content.find("boundary=");
    size_t boundary_end_pos = email_content.find('\n', boundary_pos);
    bool hasAttachment = (boundary_pos != std::string::npos);
    std::string tmp_boundary = email_content.substr(boundary_pos + 9, boundary_end_pos - boundary_pos); // my boundary has 36 character

    std::string boundary = eraseWhitespace(eraseQuotationMarks(tmp_boundary));
    
    // read header
    while (std::getline(email_stream, line))
    {
        if (line == ("--" + boundary + "\r"))
            break;
        size_t colons_pos = line.find(":");
        if (colons_pos != std::string::npos)
        {
            std::string header = line.substr(0, colons_pos);
            std::string value = line.substr(colons_pos + 2); //skip ": "
        
            if (header == "From")
            {
                email_sender = value;
            }
            else if (header == "To")
            {
                email_recipient = value;
            }
            else if (header == "CC")
            {
                std::istringstream cc_stream(value);
                std::string cc_recipient;
                while (std::getline(cc_stream, cc_recipient, ','))
                {
                    cc_list.push_back(cc_recipient);
                }
            }
            else if (header == "Subject")
            {
                subject = value;
            }
            else if (header == "Date")
            {
                date = value;
            }
        }
            
    }


    if (hasAttachment)
    {
        // message
        while (std::getline(email_stream, line) && line != "\r") {}
        
        while (std::getline(email_stream, line) && line != ("--" + boundary + "\r"))
        {
            message += eraseWhitespace(line);
        }
        

        while (std::getline(email_stream, line) && line != ("--" + boundary + "--\r"))
        {
            
            // Content-Disposition
            std::getline(email_stream, line);
            size_t filename_pos = line.find("filename=");
            std::string filename = line.substr(filename_pos + 9);
            filename = eraseQuotationMarks(filename);
            std::cout << "\nFilename: " << filename; 
            while(std::getline(email_stream, line) && line != "\r"){}
            

            std::ostringstream base64_content;
            while (std::getline(email_stream, line) && line != ("--" + boundary + "\r"))
            {
                base64_content << eraseWhitespace(line);
            }
            attachment_filename_list.push_back(filename);
            attachment_list.push_back(base64_content.str());
        }

    }
    else
    {
        while (std::getline(email_stream, line) && line != "\r") {}
        
        while (std::getline(email_stream, line) && line != (".\r"))
        {
            message += eraseWhitespace(line);
        }
    }
}

