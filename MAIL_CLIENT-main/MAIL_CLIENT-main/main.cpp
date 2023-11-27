#include "POP3Client.h"
#include "SMTPClient.h"
#include "SaveObject.h"

int main()
{

    SMTPClient local_server("127.0.0.1", 2500);

    std::string sender = "minhdang@fit.hcmus.edu.vn";
    std::string recipient = "minhdang@fit.hcmus.edu.vn";
    std::string subject = "First mail from my client";
    std::string body = "Helo";

    Email email(sender, recipient, subject, body);
    email.addBcc(sender);
    email.addCc("minhdang@gamil.com");
    email.attachFile("d:\\W02 - Cons-Des-Op.pdf");

    local_server.sendEmail(email);

    //-------------------------

    // POP3Client local_client("127.0.0.1", 1100);
    // std::string username = "minhdang@fit.hcmus.edu.vn";
    // std::string password = "minhdang123456";

    // local_client.login(username, password);

    // // local_client.loadDownloadedEmail(username);
    // local_client.retrieveAllEmail();
    // for (const auto& email : local_client.list_email) {
    //     std::cout << "Email:\n" << email << "\n\n";
    // }
    // saveEmail(username, local_client.list_email[15], "minhdang55.txt", "Importance");
    
    // std::ifstream file ("D:\\Code\\C++\\MMT\\Socket\\Source code\\database\\minhdang@fit.hcmus.edu.vn\\Importance\\minhdang55.txt");
    // std::ostringstream file_content;
    // if (file.is_open()) {
    //     file_content << file.rdbuf();
    //     file.close();
    // } 

    // Email email;
    // email.loadEmail(file_content.str());
    // // local_client.saveDownloadEmail(username);

    // std::cout << "\nFrom: " << email.getSender();
    // std::cout << "\nTo: " << email.getRecipient();
    // std::cout << "\nCC: " << email.getCC();
    // std::cout << "\nSubject: " << email.getSubject();
    // std::cout << "\nMessage: " << email.getMessage();

    
    // saveFile("E:", "W02 - Cons-Des-Op.pdf", file_content.str());
    return 0;
}