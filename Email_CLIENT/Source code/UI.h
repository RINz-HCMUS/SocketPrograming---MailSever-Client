#include "Config.h"

#define ClearScreen system("cls")
#define line "--------------------\n"

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


#define RETURN 27 // ALT or ESC
#define SUMIT -32 // CTRL

string OpenFileDialog();

bool LoginWindow();

string filterMail(Email);


// download mail
void DownloadMail();

void autoDownloadMail();

// 
void InboxWindow();

void ReceiveMail();

void WorkMailWindow();

void ImportantMailWindow();

void SpamMailWindow();

void ComposeMailWindow();

void SelectionList();

void MailBoxWindow();


