#include "UI.h"
#include "SMTPClient.h"

bool LoginWindow(){
    // Load data
    std::cout << "Welcom to Email Cliemt" << std::endl;
    std::cout << "Loading data..." << std::endl;
    CreateDatabase();
    CreateWorkTable();
    CreateImportantTable();
    CreateSpamTable();

    vector<Account> AccountList;
    SelectDatabase(&AccountList);
    std::cout << "OK1\n";

    SelectWorkKeywords(&WorkKeywords);
    std::cout << "OK2\n";

    SelectImportantKeywords(&ImportantKeyworks);
    std::cout << "OK3\n";

    SelectSpamKeywords(&SpamKeyworks);
    std::cout << "Loaded successfully!" << std::endl;
    ClearScreen;

    // Login
    std::cout << "Email Client\n";
    std::cout << line;
    std::cout << "Login\n";

    std::cout << "Email: ";
    std::getline(std::cin, UserEmail);
    
    std::cout << "Password: ";
    std::getline(std::cin, UserPassword);

    bool isLoginCompleted = false;

    for(auto account: AccountList){
        if(UserEmail == account.getEmail()){
            int tmp = 4;
            while(tmp){
                tmp--;
                if(UserPassword == account.getPassword()){
                    UserName = account.getName();
                    isLoginCompleted = true;
                    break;
                }
                else{
                    std::cout << "Wrong password! Try again: " << tmp << std::endl;
                    std::cout << "Password: ";
                    
                    std::getline(std::cin, UserPassword);
                }
            }
            if(isLoginCompleted){
                std::cout << "Completed login!" << std::endl;
                return true;
            }
            else{
                std::cerr << "Failed Login!" << std::endl;
                return false;
            }
        }
    }
    char Ans;
    std::cout << "Account does not exist" << std::endl;
    std::cout << "Register? (Y/N)" << std::endl;
    cin >> Ans;
    if(Ans != 'Y'){
        return false;
    }
    else{
        std::cout << "Your name: ";
        std::cin.ignore();
        std::getline(std::cin, UserName);

        std::cout << "Password: ";
        std::getline(std::cin, UserPassword);
        InsertDatabase(UserEmail, UserPassword, UserName, UserAutoload);
        std::cout << "Successfully registered" << std::endl;
            
        return true;
    }

    return false;
}

std::vector<std::pair<std::string, Email> > getEmailListFromFolder(std::string filter){
    std::string folderPath = "database/" + UserEmail + "/" + filter;
    std::vector<std::pair<std::string, Email> > emailList;
    std::pair<std::string, Email> tmp;
    try{
        // Lặp qua tất cả các tệp trong thư mục
        for(const auto& entry : fs::directory_iterator(folderPath)){
            // Kiểm tra xem đối tượng là một tệp tin
            if(fs::is_regular_file(entry.path())){
                // Kiểm tra xem có phải là tệp có phần mở rộng .txt không
                std::string filename  = entry.path().filename().string();
                std::string extension = entry.path().extension().string();

                
               
                    std::ifstream file (entry.path());
                    std::ostringstream file_content;
                    if (file.is_open()){
                        file_content << file.rdbuf();
                        file.close();
                    } 
                    tmp.first  = filename;
                    tmp.second = Email(file_content.str());
                    emailList.push_back(tmp);
    
            }
        }
    }
    catch (const std::exception& ex){
        std::cerr << "Error: " << ex.what() << std::endl;
    }

    return emailList;
}
void displayEmailList(std::vector<std::pair<std::string, Email> > List){
    ClearScreen;
    cout << "Loading data..." << std::endl;

    // load data (status of Email(s))
    vector<pair<string, string> > EmailList;
    std::string filepath = "database";
    filepath = filepath + "/" + UserEmail + "/" + "EmailList.txt";
    std::ifstream ifs(filepath);
    if(ifs.is_open()){
        string id, status;
        while(!ifs.eof()){
            ifs.ignore();
            ifs >> id >> status;
            EmailList.push_back({id, status});
        }
    }
    ifs.close();
    
    cout << "Loaded successfully!" << std::endl;
    ClearScreen;

    // display on screen
    for(int i = 0; i < List.size(); i++){
        cout << line;
        cout << "Email #" << (i + 1) << std::endl;
        cout << "From: " << List[i].second.getSender() << std::endl;
        cout << "Subject: " << List[i].second.getSubject() << std::endl;
        
        for(int j = 0; j < EmailList.size(); j++){
            if(EmailList[j].first == List[i].first){
                cout << "Status: " << EmailList[j].second << std::endl;
                break;
            }
        }
    }
}


void displayEmail(std::pair<std::string, Email> email, int stt){
    ClearScreen;
    cout << "Loading data..." << std::endl;
    
    // Mark as read
    vector<pair<string, string> > EmailList;
    std::string filepath = "database";
    filepath = filepath + "/" + UserEmail + "/" + "EmailList.txt";
    std::ifstream ifs(filepath);
    if(ifs.is_open()){
        string id, status;
        while(!ifs.eof()){
            ifs.ignore();
            ifs >> id >> status;
            EmailList.push_back({id, status});
        }
    }
    ifs.close();

    cout << "Load #1 OK" << endl;

    for(auto &tmp: EmailList){
        if(email.first.find(tmp.first) != std::string::npos){
            tmp.second = "read"; // mark
            break;
        }
    }

    cout << "Load #2 OK" << endl;

    ofstream ofs(filepath);
    for(auto tmp : EmailList){
        ofs << std::endl;
        ofs << tmp.first << " " << tmp.second;
    }
    ofs.close();

    cout << "Loaded Successfully!" << std::endl;
    ClearScreen;
    
    // display to screen
    cout << "Email #" << (stt + 1) << std::endl;
    cout << "Date: " << email.second.getDate() << std::endl;
    cout << "From: " << email.second.getSender() << std::endl;
    cout << "CC: " << email.second.getCC() << std::endl;
    cout << "Subject: " << email.second.getSubject() << std::endl;
    cout << "Content: " << email.second.getMessage() << std::endl;
    
    // Download attachment(s)
    vector<pair<string, string> > attachfile = email.second.getAttachment();
    char s;
    if(!attachfile.empty()){
        cout << "This mail has attachment(s). Do you want to download it? (Y/N)";
        cin >> s;
        if(s == 'Y')
            for(const auto& file: attachfile){
                saveFile("D:", file.second, file.first);
            }
    }
        
    cout << line << "Enter ESC to back";  
    while(true){
        s = _getch();
        if(s == ESC)
            return;
    }
}

void InboxWindow(){
    int stt;
    std::vector<std::pair<std::string, Email> > InboxList = getEmailListFromFolder("Inbox");
    
    std::cout << "Inbox:\n" << line;

    displayEmailList(InboxList);
  
    std::cout << line;
    std::cout << "Your choice: ";
    std::cin >> stt;
    if(stt == 0)
        return;
    stt--;
    displayEmail(InboxList[stt], stt);
}


void WorkMailWindow(){
    int stt;
    std::vector<std::pair<std::string, Email> > WorkList = getEmailListFromFolder("Work");
    
    std::cout << "Work:\n" << line;

    displayEmailList(WorkList);
  
    std::cout << line;
    std::cout << "Your choice: ";
    std::cin >> stt;
    if(stt == 0)
        return;

    stt--;
    displayEmail(WorkList[stt], stt);

}

void ImportantMailWindow(){
    int stt;
    std::vector<std::pair<std::string, Email> > ImportantList = getEmailListFromFolder("Important");
    
    std::cout << "Important:\n" << line;

    displayEmailList(ImportantList);
  
    std::cout << line;
    std::cout << "Your choice: ";
    std::cin >> stt;
    if(stt == 0)
        return;
    stt--;
    displayEmail(ImportantList[stt], stt);
}

void SpamMailWindow(){
    int stt;
    std::vector<std::pair<std::string, Email> > SpamList = getEmailListFromFolder("Spam");
    
    std::cout << "Spam:\n" << line;

    displayEmailList(SpamList);
  
    std::cout << line;
    std::cout << "Your choice: ";
    std::cin >> stt;
    if(stt == 0)
        return;
    stt--;
    displayEmail(SpamList[stt], stt);

}

void SelectionAccountList(){
    ClearScreen;
    std::cout << "Hi, " << UserName << std::endl;
    std::cout << line;
    std::cout << "Enter numbers from the keyboard to select" << std::endl;
    std::cout << line;
    
    std::cout << "1. Compose mail\n";
    std::cout << "2. Inbox\n";
    std::cout << "3. Work\n";
    std::cout << "4. Important\n";
    std::cout << "5. Spam\n";
    std::cout << "0. Log out\n";
    std::cout << "Your choice: ";
}

void MailBoxWindow(){  
    // Tạo luồng để tự động tải mail

    SelectionAccountList();

    int selected;
    std::cin >> selected;
    switch (selected){
        case 1:{
            ComposeMailWindow();
            break;
        }
        case 2:{
            InboxWindow();
            break;
        }
        case 3:{
            WorkMailWindow();
            break;
        }
        case 4:{
            ImportantMailWindow();
            break;
        }
        case 5:{
            SpamMailWindow();
            break;
        }
        case 0:{
            exit(0);
        }
        default:
            break;
    }

}

std::string OpenFileDialog(){
    OPENFILENAMEA ofn;
    char szFile[MAX_PATH] = {0};

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile) / sizeof(szFile[0]);
    ofn.lpstrFilter = "All Files (*.*)\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if(GetOpenFileNameA(&ofn)){
        return std::string(szFile);
    } 
    else{ 
        return "";
    }
}


void ComposeMailWindow(){
    // connect to Sever
    SMTPClient local_server("127.0.0.1", 2500);

    string sender = UserEmail, recipient, subject, body;
    string Receiver, tmp;
    vector<string> CC, BCC, AttachFile;

    cout << "Compose mail\n" << line;

    // Recipient
    cout << "To: ";
    cin.ignore();
    getline(cin, recipient);
    cout << line;
    // CC
    cout << "CC: ";
    getline(cin, Receiver);
    stringstream ssCC(Receiver);
    while (ssCC >> tmp){
        CC.push_back(tmp);
    }
    ssCC.clear();
    cout << line;
    // BCC
    cout << "BCC: ";
    getline(cin, recipient);
    stringstream ssBCC(Receiver);
    while (ssBCC >> tmp){
        BCC.push_back(tmp);
    }
    
    ssBCC.clear();
    cout << line;
    // Subject
    cout << "Subject: ";
    getline(cin, subject);

    // Body content
    cout << "Content: (please type 'end!' on a new line to finish)\n";
    std::string content;
    while(std::getline(std::cin, content) && content != "end!") {
        body += content + "\n"; 
    }
    cout << line;

    // Attach file
    cout << "Attach file: \n";
    while(tmp != ""){    
        tmp = OpenFileDialog();
        cout << tmp << endl;
        AttachFile.push_back(tmp);
    }

    cout << "Send mail? (Y/N)\n";
    getline(cin, tmp);
    if(tmp == "Y")
        cout << "Sending...\n";
    else{
        cout << "Failed to send email.\n";
        return;
    }

    // send mail
    Email email(sender, recipient, subject, body);

    for(auto R: CC)
        email.addCc(R);
    
    for(auto R: BCC)
        email.addBcc(R);

    for(auto R: AttachFile)
        email.attachFile(R);

    local_server.sendEmail(email);

    cout << "Email sent successfully!\n";
    Sleep(2500);
}


void autoDownloadMail(){   
    while(true){
        POP3Client local_client("127.0.0.1", 1100);
        std::string username = UserEmail;
        std::string password = UserPassword;

        local_client.login(username, password);
        local_client.loadDownloadedEmail(username);

        std::string folderName = "database";
        folderName = folderName + "/" + UserEmail + "/" + "EmailList.txt";
        
        // load downloaded mail
        std::vector<std::string> DownloadedMail;
        std::ifstream ifs(folderName);
        if(!ifs.is_open()){
            std::ofstream fs(folderName);
            fs.close();
        }
        else{
            std::string tmp;
            while(!ifs.eof()){
                ifs.ignore();
                ifs >> tmp;
                DownloadedMail.push_back(tmp);
                ifs >> tmp;
            }
        }
        ifs.close();

        std::ofstream ofs(folderName, std::ios::app);
        if(!ofs.is_open())
            cout << "Error open file" << endl;
        // load mail on client
        std::vector<std::pair<std::string,std::string> > AccountList_email = local_client.retrieveAllEmail();

        // check and download
        for (const auto& email : AccountList_email){
            if(std::find(DownloadedMail.begin(), DownloadedMail.end(), email.second) == DownloadedMail.end()){
                string filter = filterMail(email.first);
                ofs << endl << " " << email.second << " unread";
                saveEmail(username, email.first, email.second, filter);
            }
        }
        
        ofs.close();

        //cout << "Downloaded successfully" << endl;
        
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }
}

string filterMail(Email Mail){
    string content = Mail.getSender();
    content += Mail.getSubject();
    content += Mail.getMessage();
    
    for(string word: WorkKeywords)
        if(content.find(word) != std::string::npos)
            return "Work";

    for(string word: ImportantKeyworks)
        if(content.find(word) != std::string::npos)
            return "Important";
    
    for(string word: SpamKeyworks)
        if(content.find(word) != std::string::npos)
            return "Spam";

    return "Inbox";
}