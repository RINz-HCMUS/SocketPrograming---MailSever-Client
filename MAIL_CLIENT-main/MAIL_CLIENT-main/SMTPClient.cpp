#include "SMTPClient.h"


void SMTPClient::sendEmail(const Email& email) 
{

    if (!connectToServer()) {
        std::cerr << "Failed to connect to the SMTP server." << std::endl;
        return;
    }

    // Perform SMTP handshake
    sendCommand("EHLO [127.0.0.1] \r\n");
    receiveResponse();

    // Specify sender
    sendCommand("MAIL FROM: <" + email.getSender() + ">\r\n");
    receiveResponse();

    // Specify recipient
    sendCommand("RCPT TO: <" + email.getRecipient() + ">\r\n");
    receiveResponse();

    std::vector<std::string> cc_list = email.getListCC();
    std::vector<std::string> bcc_list = email.getListBCC();
    // CC BCC
    for (const auto& recipient : cc_list)
    {
        sendCommand("RCPT TO: <" + recipient + ">\r\n");
        receiveResponse();
    }
    for (const auto& recipient : bcc_list)
    {
        sendCommand("RCPT TO: <" + recipient + ">\r\n");
        receiveResponse();
    }


    // Start email data
    sendCommand("DATA\r\n");
    receiveResponse();

    // Send email content
    sendCommand(email.formatEmail());
    
    sendCommand(".\r\n");

    // Quit session
    sendCommand("QUIT\r\n");
    receiveResponse();
}



