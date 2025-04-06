#include <stdio.h>
#include <iostream>
#include <filesystem>
#include <vector>
#include <fstream>

namespace fs = std::filesystem;

bool login(fs::path user){
    std::string name;
    std::string email;

    std::ifstream file(user);
    std::string line;
    int i = 0;
    if(!getline(file, name)) return false;
    if(!getline(file, email)) return false;
    system("git status");
    return true;
}

int main(int argc, char** argv){
    fs::path users_dir = fs::current_path().root_path().append("gitusers");
    fs::create_directory(users_dir);

    std::vector<fs::path> users;
    for(const auto & entry : fs::directory_iterator(users_dir)){
        users.push_back(entry.path());
    }

    if(users.size() == 0){
        printf("No users available.\n");
        return 1;
    }

    for(int i = 0; i < users.size(); i++){
        printf("%d: %s\n", i, users[i].filename().string().c_str());
        // std::cout << i << ": " << users[i] << "\n";
    }
    if(users.size() == 1) printf("Select a user (0): ");
    else printf("Select a user (0-%d): ", users.size()-1);
    std::string input;
    std::cin >> input;
    unsigned int user_i;
    try{
        user_i = std::stoi(input);
    }catch(...){
        printf("Invalid input.\n");
        return 1;
    }

    if(user_i >= users.size()){
        printf("Invalid selection.");
        return 1;
    }

    if(!login(users[user_i])){
        printf("Failed to login to \"%s\"\n", users[user_i].filename().string().c_str());
    }
    
    printf("User \"%s\" selected.\n", users[user_i].filename().string().c_str());

    return 0;
}