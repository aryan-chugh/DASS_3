#ifndef YADAAPPLICATION_H
#define YADAAPPLICATION_H

#include <memory>
#include <string>
#include "../authentication/UserManager.h"
#include "../database/FoodDatabase.h"
#include "../logging/LogManager.h"
#include "../command/CommandProcessor.h"
#include "../authentication/User.h"

class YADAApplication {
public:
    YADAApplication();
    void start();
    void loadData();
    void saveAll();

private:
    bool authenticateUser();
    bool login();
    bool registerUser(); // renamed because register is reserved
    void displayMenu();
    void displayFoodManager();
    void displayDailyUpdateMenu();
    void displayProfileView();
    void displayUserProfile();

    bool processCommand(const std::string &choice);
    bool processCommandFood(const std::string &choice);
    bool processCommandDaily(const std::string &choice);
    bool processCommandProfile(const std::string &choice);

    void viewFoodDatabase();
    void addBasicFood();
    void createCompositeFood();
    void addFoodToLog();
    std::shared_ptr<FoodItem> selectFood();
    void viewDailyLog();
    void removeFoodFromLog();
    void viewDietStatistics();
    void updateDailyStats();
    void changeCalorieCalculator();
    void changeCurrentDate();
    void undoLastAction();
    void updateFoodInLog();

    std::unique_ptr<UserManager> userManager;
    std::unique_ptr<FoodDatabase> foodDatabase;
    std::unique_ptr<LogManager> logManager;
    std::unique_ptr<CommandProcessor> commandProcessor;
    std::shared_ptr<User> currentUser;
    std::string currentDate; // Format "yyyy-MM-dd"
};

#endif // YADAAPPLICATION_H
