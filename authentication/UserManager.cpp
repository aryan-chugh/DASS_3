#include "UserManager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#ifdef _WIN32
    #include <direct.h>
#else
    #include <sys/stat.h>
#endif
#include "../diet/DailyStats.h"
#include "../diet/HarrisBenedictCalculator.h"
#include "../diet/MifflinStJeorCalculator.h"

UserManager::UserManager() {}

std::shared_ptr<User> UserManager::registerUser(const std::string &username, const std::string &password) {
    if (users.find(username) != users.end())
        return nullptr;
    auto user = std::make_shared<User>(username, password);
    // Set default profile values
    user->getDietProfile()->setCalorieCalculator(std::make_shared<HarrisBenedictCalculator>());
    users[username] = user;
    return user;
}

std::shared_ptr<User> UserManager::loginUser(const std::string &username, const std::string &password) {
    if (users.find(username) != users.end()) {
        auto user = users[username];
        if (user->authenticate(password))
            return user;
    }
    return nullptr;
}

void UserManager::saveUsers() {
    // Create data/users directory if it doesn't exist
    #ifdef _WIN32
        _mkdir("data");
        _mkdir("data/users");
    #else
        mkdir("data", 0777);
        mkdir("data/users", 0777);
    #endif

    // Open main user file
    std::ofstream ofs(USER_FILE);
    if (!ofs) {
        throw std::runtime_error("Could not open user file for writing: " + std::string(USER_FILE));
        return;
    }

    for (const auto &pair : users) {
        auto user = pair.second;
        if (!user) continue;  // Skip if user pointer is null

        try {
            // Save main user data into USER_FILE with calculator type
            std::string calculatorType = 
                dynamic_cast<HarrisBenedictCalculator*>(user->getDietProfile()->getCalorieCalculator().get()) ? 
                "HB" : "MSJ";

            ofs << user->getUsername() << "," 
                << user->getPasswordHash() << ","
                << user->getDietProfile()->getGender() << ","
                << user->getDietProfile()->getHeight() << ","
                << user->getDietProfile()->getAge() << ","
                << calculatorType << "\n";

            // Create individual stats file for each user
            std::string statsFilePath = "data/users/" + user->getUsername() + "_stats.txt";
            std::ofstream statsFile(statsFilePath);
            if (!statsFile) {
                throw std::runtime_error("Could not open stats file: " + statsFilePath);
                continue;
            }

            // Write daily stats
            auto allStats = user->getDietProfile()->getAllDailyStats();
            for (const auto &entry : allStats) {
                if (!entry.second) continue;
                
                statsFile << entry.first << ","
                         << entry.second->getWeight() << ","
                         << static_cast<int>(entry.second->getActivityLevel()) << ","
                         << entry.second->getAge() << "\n";
            }

            statsFile.close();
        } catch (const std::exception &e) {
            std::cerr << "Error saving user " << user->getUsername() << ": " << e.what() << std::endl;
        }
    }

    ofs.close();
}

void UserManager::loadUsers() {
    users.clear();
    std::ifstream ifs(USER_FILE);
    if (!ifs) {
        std::cerr << "Could not open user file for reading: " << USER_FILE << std::endl;
        return;
    }

    std::string line;
    while (std::getline(ifs, line)) {
        try {
            std::istringstream iss(line);
            std::string username, pass, gender, heightStr, ageStr, calculatorType;
            std::getline(iss, username, ',');
            std::getline(iss, pass, ',');
            std::getline(iss, gender, ',');
            std::getline(iss, heightStr, ',');
            std::getline(iss, ageStr, ',');
            std::getline(iss, calculatorType, ',');

            auto user = std::make_shared<User>(username, pass);
            user->getDietProfile()->setGender(gender);
            user->getDietProfile()->setHeight(std::stod(heightStr));
            user->getDietProfile()->setAge(std::stoi(ageStr));
            
            // Set the appropriate calculator
            if (calculatorType == "HB") {
                user->getDietProfile()->setCalorieCalculator(
                    std::make_shared<HarrisBenedictCalculator>()
                );
            } else {
                user->getDietProfile()->setCalorieCalculator(
                    std::make_shared<MifflinStJeorCalculator>()
                );
            }

            users[username] = user;

            // Load age changes from stats file
            std::string statsFilePath = "data/users/" + username + "_stats.txt";
            std::ifstream statsFile(statsFilePath);
            if (statsFile) {
                std::string statLine;
                while (std::getline(statsFile, statLine)) {
                    std::istringstream statStream(statLine);
                    std::string date, weightStr, activityStr, ageStr;
                    std::getline(statStream, date, ',');
                    std::getline(statStream, weightStr, ',');
                    std::getline(statStream, activityStr, ',');
                    std::getline(statStream, ageStr, ',');  // Optional age field

                    try {
                        double weight = std::stod(weightStr);
                        int activity = std::stoi(activityStr);
                        int age = ageStr.empty() ? user->getDietProfile()->getAge() : std::stoi(ageStr);
                        
                        auto stats = std::make_shared<DailyStats>();
                        stats->setWeight(weight);
                        stats->setActivityLevel(static_cast<ActivityLevel>(activity));
                        stats->setAge(age);                                    
                        user->getDietProfile()->updateDailyStats(date, stats->getAge(), weight, activity);
                        user->getDietProfile()->setAge(age);  // Update user's age in profile
                    } catch (const std::invalid_argument& e) {
                        std::cerr << "Invalid data in stats file for user " << username << ": " << e.what() << std::endl;
                        continue;
                    } catch (const std::out_of_range& e) {
                        std::cerr << "Out of range error in stats file for user " << username << ": " << e.what() << std::endl;
                        continue;
                    } catch (const std::exception& e) {
                        continue;
                    }
                }
                statsFile.close();
            }
        } catch (const std::exception& e) {
            continue;
        }
    }

    ifs.close();
}
