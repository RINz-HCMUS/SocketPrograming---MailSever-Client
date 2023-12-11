#include "Config.h"

Account::Account(std::string EMAIL, std::string PASS, std::string NAME, int AUTOLOAD) {
    Email = EMAIL;
    Password = PASS;
    Name = NAME;
    Autoload = AUTOLOAD;
}

void Account::printInfor(){
    cout << "Email: "       << Email    << " "
         << "Name: "        << Name     << " "
         << "Password: "    << Password << " "
         << "Autoload: "    << Autoload << " s" << std::endl;
}

string Account::getEmail(){
    return Email;
}

string Account::getPassword(){
    return Password;
}

string Account::getName(){
    return Name;
}
static int callback(void *NotUsed, int argc, char **argv, char **azColName){
    for(int i=0; i<argc; i++){
        if (argv[i] != nullptr) {
            std::cout << azColName[i] << " = " << argv[i] << std::endl;
        } else {
            std::cout << azColName[i] << " = NULL" << std::endl;
        }
    }

    printf("\n");
    return 0;
}

static int loadback(void *data, int argc, char **argv, char **azColName){
    std::vector<Account>* List = static_cast<std::vector<Account>*>(data);

    int autoload = std::stoi(argv[3]);
    Account Ex(argv[0], argv[1], argv[2], autoload);
    List->emplace_back(Ex);

    return 0;
}

static int loadKeywords(void *data, int argc, char **argv, char ** azColName){
    std::vector<string>* List = static_cast<std::vector<string>*>(data);

    List->emplace_back(argv[0]);

    return 0;
}

void CreateDatabase(){
    sqlite3 *db;
    char* zErrMsg = 0;
    int rc;

    rc = sqlite3_open("Config.db", &db);

    if(rc){
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;    
        exit(0);
    }


    // Create SQL
    std::string sql;
    sql = "CREATE TABLE IF NOT EXISTS ACCOUNT(" \
      "EMAIL TEXT PRIMARY KEY   NOT NULL," \
      "PASSWORD TEXT            NOT NULL," \
      "NAME     TEXT            NOT NULL," \
      "AUTOLOAD INT             DEFAULT 10);";

    

    // Execute SQL
    rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
    if( rc != SQLITE_OK){
        std::cerr << "Database error: " << zErrMsg;
        sqlite3_free(zErrMsg);
    }


    sqlite3_close(db);
}

void InsertDatabase(string email, string password, string name, int autoload){
    sqlite3 *db;
    char* zErrMsg = 0;
    int rc;

    rc = sqlite3_open("Config.db", &db);

    if(rc){
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;    
        exit(0);
    }

    // Insert
    string sql;

    sql = "INSERT INTO ACCOUNT (EMAIL, PASSWORD, NAME, AUTOLOAD) VALUES ('" +
        email + "', '" + password + "', '" + name + "', " + std::to_string(autoload) + ");";

    // Thuc thi cau lenh SQL
  

    rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
    if(rc == SQLITE_OK){
        std::cout << "Records created sucessfully" << endl;
    }
    else{
        std::cerr << "SQL error: " << zErrMsg << std::endl;
    }

    sqlite3_close(db);
}



void SelectDatabase(vector<Account>* List){
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    std::string sql;
    const char* data = "Callback function called";

    /* Open database */
    rc = sqlite3_open("Config.db", &db);
    if( rc ){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        exit(0);
    }

    /* Create SQL statement */
    sql = "SELECT * from ACCOUNT";

    /* Execute SQL statement */
    
    rc = sqlite3_exec(db, sql.c_str(), loadback, List, nullptr);

    // rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    sqlite3_close(db);
}

void SelectWorkKeywords(vector<string>* List){
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    string sql;
    const char* data = "Callback function called";

    rc = sqlite3_open("Config.db", &db);
    if(rc){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        exit(0);
    }

    sql = "SELECT * from WORK";

    rc = sqlite3_exec(db, sql.c_str(), loadKeywords, List, nullptr);

    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    sqlite3_close(db);
}

void SelectImportantKeywords(vector<string>* List){
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    string sql;
    const char* data = "Callback function called";

    rc = sqlite3_open("Config.db", &db);
    if(rc){
        std::cerr << "Cannot open database: " << sqlite3_errmsg(db) << std::endl;
        exit(0);
    }

    sql = "SELECT * from IMPORTANT";

    rc = sqlite3_exec(db, sql.c_str(), loadKeywords, List, nullptr);

    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    sqlite3_close(db);
}

void SelectSpamKeywords(vector<string>* List){
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    string sql;
    const char* data = "Callback function called";

    rc = sqlite3_open("Config.db", &db);
    if(rc){
        std::cerr << "Cannot open database: " << sqlite3_errmsg(db) << std::endl;
        exit(0);
    }

    sql = "SELECT * from SPAM";

    rc = sqlite3_exec(db, sql.c_str(), loadKeywords, List, nullptr);

    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    sqlite3_close(db);
}

void CreateWorkTable(){
    sqlite3 *db;
    char* zErrMsg = 0;
    int rc;

    rc = sqlite3_open("Config.db", &db);

    if(rc){
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;    
        exit(0);
    }

    // Create Work key list
    string createWorkTable;
    createWorkTable = "CREATE TABLE IF NOT EXISTS WORK(" \
            "KEYWORD TEXT PRIMARY KEY NOT NULL);" ;
    
    rc = sqlite3_exec(db, createWorkTable.c_str(), callback, 0, &zErrMsg);
    
    if( rc != SQLITE_OK){
        std::cerr << "Work error: " << zErrMsg;
        sqlite3_free(zErrMsg);
    }


    //sqlite3_close(db);


    std::vector<std::string> WorkKeywords = {"test", "project", "task", "deadline"};

    for (const auto& keyword : WorkKeywords){
        std::string insertSQL = "INSERT OR IGNORE INTO WORK (KEYWORD) VALUES ('" + keyword + "');";

        rc = sqlite3_exec(db, insertSQL.c_str(), nullptr, nullptr, &zErrMsg);

        if(rc != SQLITE_OK){
            std::cerr << "Error inserting keyword: " << zErrMsg << std::endl;
            sqlite3_free(zErrMsg);
        }
    }

    sqlite3_close(db);

    return;
}

void CreateImportantTable(){
    sqlite3* db;
    char* zErrMsg = 0;
    int rc;
    
    rc = sqlite3_open("Config.db", &db);

    if(rc){
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;    
        exit(0);
    }

    std::string createImportantTable;
    createImportantTable = "CREATE TABLE IF NOT EXISTS IMPORTANT(" \
            "KEYWORD TEXT PRIMARY KEY NOT NULL);";

    rc = sqlite3_exec(db, createImportantTable.c_str(), callback, 0, &zErrMsg);

    if( rc != SQLITE_OK){
        std::cerr << "Important error: " << zErrMsg;
        sqlite3_free(zErrMsg);
    }

    // Assume that importantKeywords is a std::vector<std::string> containing the keywords to insert
    std::vector<std::string> importantKeywords = {"important", "priority", "important"};

    for (const auto& keyword : importantKeywords) {
        std::string insertSQL = "INSERT OR IGNORE INTO IMPORTANT (KEYWORD) VALUES ('" + keyword + "');";

        rc = sqlite3_exec(db, insertSQL.c_str(), nullptr, nullptr, &zErrMsg);

        if (rc != SQLITE_OK) {
            std::cerr << "Error inserting keyword: " << zErrMsg << std::endl;
            sqlite3_free(zErrMsg);
        }
    }

    sqlite3_close(db);

    return;
}

void CreateSpamTable(){
    sqlite3* db;
    char* zErrMsg = 0;
    int rc;
    
    rc = sqlite3_open("Config.db", &db);

    if (rc != SQLITE_OK){
        std::cerr << "Cannot open database: " << sqlite3_errmsg(db) << std::endl;
        exit(0);
    }

    
    std::string createSpamTable;
    createSpamTable = "CREATE TABLE IF NOT EXISTS SPAM(" \
            "KEYWORD TEXT PRIMARY KEY NOT NULL);";

    rc = sqlite3_exec(db, createSpamTable.c_str(), callback, 0, &zErrMsg);

    if (rc != SQLITE_OK) {
        std::cerr << "Error creating table: " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
    }

    // Assume that spamKeywords is a std::vector<std::string> containing the keywords to insert
    std::vector<std::string> spamKeywords = {"sale", "offer", "free", "prize"};

    for (const auto& keyword : spamKeywords) {
        std::string insertSQL = "INSERT OR IGNORE INTO SPAM (KEYWORD) VALUES ('" + keyword + "');";

        rc = sqlite3_exec(db, insertSQL.c_str(), nullptr, nullptr, &zErrMsg);

        if (rc != SQLITE_OK) {
            std::cerr << "Error inserting keyword: " << zErrMsg << std::endl;
            sqlite3_free(zErrMsg);
        } 
    }

    sqlite3_close(db);

    return;
}