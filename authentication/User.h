#ifndef USER_H
#define USER_H

#include <string>
#include <memory>
#include "../diet/DietProfile.h"

class User {
public:
    User(const std::string &username, const std::string &passwordHash);
    std::string getUsername() const;
    bool authenticate(const std::string &password) const;
    std::shared_ptr<DietProfile> getDietProfile() const;
    std::string getPasswordHash() const;
private:
    std::string username;
    std::string passwordHash;
    std::shared_ptr<DietProfile> dietProfile;
    std::string hashPassword(const std::string &password) const;
};

#endif // USER_H
