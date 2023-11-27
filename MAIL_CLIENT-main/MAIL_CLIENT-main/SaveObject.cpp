#include "SaveObject.h"
#include <iostream>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

namespace fs = std::filesystem;

void saveEmail(const std::string& username, const std::string& email_content, const std::string& filename, const std::string& subfolder)
{
    std::string database_folder = "database";
    std::string user_folder = database_folder + "/" + username;
    std::string deeper_folder = user_folder + "/" + subfolder;  
    std::string file_path = deeper_folder + "/" + filename;

    if (!fs::exists(database_folder))
    {
        fs::create_directory(database_folder);
    }

    if (!fs::exists(user_folder))
    {
        fs::create_directory(user_folder);
    }

    if (!fs::exists(deeper_folder))
    {
        fs::create_directory(deeper_folder);
    }

    std::ofstream file(file_path);

    if (file.is_open())
    {
        file << email_content;
        file.close();
    }
    else
    {
        std::cerr << "Error saving email in folder: " << file_path << "\n";
    }
}

void saveFile(const std::string& file_path, const std::string& filename, const std::string& file_content)
{
    std::vector<BYTE> decoded_content = base64_decode(file_content);

    std::ofstream file(file_path + "/" + filename, std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(decoded_content.data()), decoded_content.size());
        file.close();
        std::cout << "File saved successfully: " << file_path << "/" << filename << std::endl;
    } else {
        std::cerr << "Error: Unable to open file for writing: " << file_path << "/" << filename << std::endl;
    }
}




