#include <stdio.h>
#include <iostream>
#include <filesystem>
#include <vector>
#include <fstream>
#include <cstring>
#include <string>

namespace fs = std::filesystem;

void print_current_config(){
    system("git config user.name");
    system("git config user.email");
}

bool is_in_repo(){
    return system("git status >nul 2>nul") == 0;
}

bool is_git_available(){
    return system("git --version >nul 2>nul") == 0;
}

int login(fs::path user){
    std::string name;
    std::string email;

    std::ifstream file(user);
    std::string line;
    int i = 0;
    if(!std::getline(file, name)) return 1;
    if(!std::getline(file, email)) return 1;
    name = "git config user.name \""+name+"\"";
    email = "git config user.email \""+email+"\"";
    system(name.c_str());
    system(email.c_str());
    return 0;
}

int add_user(std::vector<fs::path> users){
    std::string id;
    std::string name;
    std::string email;
    printf("Profile name: ");
    std::cin >> id;
    printf("Username: ");
    std::cin >> name;
    printf("Email: ");
    std::cin >> email;

    if(id == "add" || id == "remove" || id == "list"){
        printf("ERROR: Cannot name profile \"%s\".\n", id.c_str());
        return 1;
    }

    for(int i = 0; i < users.size(); i++){
        if(users[i].filename().string() == id) {
            printf("ERROR: Profile of that name already exists.\n");
            return 1;
        }
    }

    std::ofstream file;
    file.open(fs::current_path().root_path().append("gitusers").append(id));
    file << name << "\n";
    file << email;
    file.close();

    return 0;
}

int main(int argc, char** argv){
    if(!is_git_available()){
        printf("ERROR: git not found.\n");
        return 1;
    }

    fs::path users_dir = fs::current_path().root_path().append("gituser");
    fs::create_directory(users_dir);

    std::vector<fs::path> users;
    for(const auto & entry : fs::directory_iterator(users_dir)){
        if(entry.path().extension().string() == ""){
            users.push_back(entry.path());
        }
    }

    if(argc >= 2){
        if(strcmp(argv[1], "add") == 0){
            return add_user(users);
        }else if(strcmp(argv[1], "remove") == 0){ // TODO
            if(argc < 3){
                printf("ERROR: No user specified.\n");
                return 1;
            }
            return 0;
        }else if(strcmp(argv[1], "list") == 0){ // TODO
            if(users.size() == 0){
                printf("No profiles available.\n");
            }else{
                for(int i = 0; i < users.size(); i++){
                    printf("- %s\n", users[i].filename().string().c_str());
                }
            }
            return 0;
        }else{
            if(!is_in_repo()){
                printf("ERROR: Not in repository.\n");
                return 1;
            }
            for(int i = 0; i < users.size(); i++){
                if(users[i].filename().string() == std::string(argv[1])){
                    if(login(users[i]) == 1){
                        printf("ERROR: Failed to login to \"%s\".\n", users[i].filename().string().c_str());
                        return 1;
                    }
                    printf("Profile \"%s\" selected.\n", users[i].filename().string().c_str());
                    return 0;
                }
            }
            printf("ERROR: No profile with that name found.\n");
            return 1;
        }
    }

    print_current_config();

    return 0;
}