#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <string>
#include <map>
#include <memory>
#include "User.h"

class UserManager {
public:
    UserManager();
    std::shared_ptr<User> registerUser(const std::string &username, const std::string &password);
    std::shared_ptr<User> loginUser(const std::string &username, const std::string &password);
    void saveUsers();
    void loadUsers();
private:
    std::map<std::string, std::shared_ptr<User>> users;
    const std::string USER_FILE = "users.txt";
};

#endif // USERMANAGER_H
