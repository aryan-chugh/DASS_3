#include "User.h"

User::User(const std::string &username, const std::string &passwordHash)
    : username(username), passwordHash(passwordHash), dietProfile(std::make_shared<DietProfile>()) {}

std::string User::getUsername() const {
    return username;
}

bool User::authenticate(const std::string &password) const {
    return passwordHash == hashPassword(password);
}

std::shared_ptr<DietProfile> User::getDietProfile() const {
    return dietProfile;
}

std::string User::getPasswordHash() const {
    return passwordHash;
}

std::string User::hashPassword(const std::string &password) const {
    // In real applications, use a proper hashing function.
    return password;
}
