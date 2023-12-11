#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "comdlg32")
#pragma comment(lib, "sqlite3")

#include "POP3Client.h"
#include "SMTPClient.h"
#include "SaveObject.h"

#include <sqlite3.h>
#include <Windows.h>
#include <algorithm>
#include <conio.h>

#include <sstream>
#include <fstream>
#include <thread>
#include <chrono>

/* Keyboard */
#define ESC 27

/* Mail box */
#define LOGINWINDOW 0
#define MAILBOXWINDOW 1
#define COMPOSEMAILWINDOW 2
#define INBOXWINDOW 3
#define WORKMAILWINDOW 4
#define IMPORTANTMAILWINDOW 5
#define SPAMMAILWINDOW 6

/* Flter */
#define INBOX 0
#define IMPORTANT 1
#define WORK 2
#define SPAM 3
/* g++ *.cpp -o test.exe -lsqlite3 -lws2_32 -lcomdlg32 */
namespace fs = std::filesystem;

using namespace std;

static std::string UserEmail;
static std::string UserPassword;
static std::string UserName;
static int UserAutoload = 10;

static vector<std::string> WorkKeywords;
static vector<std::string> ImportantKeyworks;
static vector<std::string> SpamKeyworks;

class Account{
    private:
        std::string Email;
        std::string Password;
        std::string Name;
        int Autoload;
    public:
        Account() {}
        Account(std::string EMAIL, std::string PASS, std::string NAME, int AUTOLOAD);
        
        string getEmail();
        string getName();
        string getPassword();
        
        void printInfor();
};

//
static int callback(void *NotUsed, int argc, char **argv, char **azColName);

static int loadback(void *data, int argc, char **argv, char **azColName);

static int loadKeywords(void *data, int argc, char **argv, char **azColName);

// Create configuration data
void CreateDatabase();

void CreateWorkTable();

void CreateImportantTable();

void CreateSpamTable();

// load configuration data
void SelectDatabase(vector<Account>*);

void SelectWorkKeywords(vector<string>*);

void SelectImportantKeywords(vector<string>*);

void SelectSpamKeywords(vector<string>*);

// add new account to configuration data
void InsertDatabase(string email, string password, string name, int autoload);


