#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include "base64.h"

#ifndef EMAIL_H
#define EMAIL_H

class Email
{
private:
    std::string email_sender;
    std::string email_recipient;
    std::string subject;
    std::string message;
    std::string message_ID;
    std::string date;
    std::vector<std::string> cc_list;
    std::vector<std::string> bcc_list;
    std::vector<std::string> attachment_list;
    std::vector<std::string> attachment_filename_list;
    
    // Handle file_path
    std::string getFilename(const std::string& file_path) const;
    std::string getFileExtension(const std::string& file_name) const;
    std::string getContentType(const std::string& file_extension) const;
    
    // Handle mail format 
    std::string getDomain() const;
    std::string genDate() const;
    std::string genUserAgent() const;
    std::string genUniqueString(int size_string) const;
    std::string genMessageID() const;
    std::string genBoundary() const;

public:
    // constructor
    Email() : email_sender(""), email_recipient(""), subject(""), message("") {}
    
    Email(const std::string& SENDER, const std::string& TO, const std::string& SUBJECT, const std::string& MSG)
        : email_sender(SENDER), email_recipient(TO), subject(SUBJECT), message(MSG) {}
        // load email_content to class Email
        // set date, sender, recipient, subject, message, list_cc, attachment
        // use when having email_content and want to show
    Email(const std::string& email_content);

    // add cc, bcc, file_path
    void addCc(const std::string& recipient);  
    void addBcc(const std::string& recipient);
    void attachFile(const std::string& file_path);
    
    std::string formatEmail() const; // return format of mail

    // getter 
    std::string getDate() const; // get date, use to show "Date: "
    std::string getSender() const; // get sender,  use to show "From: "
    std::string getRecipient() const; // get recipient, use to show "To: "
    std::string getSubject() const; // get subject, use to show "Subject: "
    std::string getMessage() const; // get Message, use to show "Message: "
    std::string getCC() const; // get list of cc, use to show "Cc: "
    std::vector<std::string> getListCC() const; // get vector list_cc
    std::string getBCC() const;// get list of bcc, use to show "Bcc: ", but it is unecessary
    std::vector<std::string> getListBCC() const; // get vector list_bcc
        // get vector<file_content, filename> use to save attachment when using func saveFile
    std::vector<std::pair<std::string, std::string>> getAttachment() const; 

};
    
#endif // EMAIL