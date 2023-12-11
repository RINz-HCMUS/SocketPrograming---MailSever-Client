#include "base64.h"
#ifndef SAVE_OBJECT_H
#define SAVE_OBJECT_H

#include <string>
        // use to save email_content
        // saveEmail(username, email_content, name, subfolder)
        // subfolder use for filter. 
    void saveEmail(const std::string& username, const std::string& email_content, const std::string& filename, const std::string& subfolder);
        // use to save attachment
    void saveFile(const std::string& file_path, const std::string& filename, const std::string& file_content);

#endif // SAVE_OBJECT_H