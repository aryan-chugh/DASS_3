#include "YADAApplication.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iterator>
#include "../diet/ActivityLevel.h"
#include "../database/BasicFood.h"
#include "../database/CompositeFood.h"
#include "../command/AddFoodCommand.h"
#include "../command/UpdateFoodCommand.h"
#include "../command/RemoveFoodCommand.h"
#include "../diet/HarrisBenedictCalculator.h"
#include "../diet/MifflinStJeorCalculator.h"
#include "../command/AddFoodToDatabaseCommand.h"
#include "../diet/UpdateProfileCommand.h"
#include "../command/ChangeCalorieCalculatorCommand.h"
#include <memory>
#include <ctime>
#include <iomanip>

YADAApplication::YADAApplication() {
    userManager = std::make_unique<UserManager>();
    foodDatabase = std::make_unique<FoodDatabase>();
    logManager = nullptr;
    commandProcessor = std::make_unique<CommandProcessor>();
    
    // Get current date in YYYY-MM-DD format
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d");
    currentDate = oss.str();
}

void YADAApplication::start() {
    std::cout << "Welcome to YADA - Yet Another Diet Assistant!" << std::endl;
    
    // Load essential data first
    try {
        userManager->loadUsers();
        foodDatabase->loadDatabase();
    } catch (const std::exception &e) {
        std::cout << "Error loading data: " << e.what() << std::endl;
        std::cout << "Starting with empty databases." << std::endl;
    }

    // Authenticate user before loading their logs
    if (!authenticateUser()) {
        std::cout << "Authentication failed. Exiting application." << std::endl;
        return;
    }

    // Initialize and load log manager only after successful authentication
    if (currentUser) {
        logManager = std::make_unique<LogManager>(currentUser->getUsername());
        try {
            logManager->load(foodDatabase.get());  // Pass foodDatabase to restore food references
            std::cout << "User logs loaded successfully." << std::endl;
        } catch (const std::exception &e) {
            std::cout << "Error loading user logs: " << e.what() << std::endl;
            std::cout << "Starting with empty log." << std::endl;
        }
    }

    bool running = true;
    while (running) {
        displayMenu();
        std::string choice;
        std::getline(std::cin, choice);

        if(choice == "1") {
            displayFoodManager();
            std::string choice;
            std::string choice2;
            std::getline(std::cin, choice2);
            
            running = processCommandFood(choice2);
        }else if(choice == "2") {
            displayDailyUpdateMenu();
            std::string choice2;
            std::getline(std::cin, choice2);
            
            running = processCommandDaily(choice2);
        }else if(choice == "3") {
            displayProfileView();
            std::string choice2;
            std::getline(std::cin, choice2);
            
            running = processCommandProfile(choice2);
        }else {
            running = processCommand(choice);
        }
    }
    saveAll();
    std::cout << "Thank you for using YADA. Goodbye!" << std::endl;
}

void YADAApplication::loadData() {
    try {
        userManager->loadUsers();
        foodDatabase->loadDatabase();
        std::cout << "Data loaded successfully." << std::endl;
    } catch (const std::exception &e) {
        std::cout << "Error loading data: " << e.what() << std::endl;
        std::cout << "Starting with empty databases." << std::endl;
    }
}

void YADAApplication::saveAll() {
    try {
        userManager->saveUsers();
        foodDatabase->saveDatabase();
        logManager->save();
        std::cout << "All data saved successfully." << std::endl;
    } catch (const std::exception &e) {
        std::cout << "Error saving data: " << e.what() << std::endl;
    }
}

bool YADAApplication::authenticateUser() {
    std::cout << "1. Login" << std::endl;
    std::cout << "2. Register" << std::endl;
    std::cout << "Choose an option: ";
    std::string choice;
    std::getline(std::cin, choice);
    if (choice == "1")
        return login();
    else if (choice == "2")
        return registerUser();
    else {
        std::cout << "Invalid choice." << std::endl;
        return false;
    }
}

bool YADAApplication::login() {
    std::cout << "Username: ";
    std::string username;
    std::getline(std::cin, username);
    std::cout << "Password: ";
    std::string password;
    std::getline(std::cin, password);
    auto user = userManager->loginUser(username, password);
    if (user) {
        currentUser = user;
        std::cout << "Login successful. Welcome, " << username << "!" << std::endl;
        return true;
    } else {
        std::cout << "Login failed. Invalid username or password." << std::endl;
        return false;
    }
}

bool YADAApplication::registerUser() {
    std::cout << "Create username: ";
    std::string username;
    std::getline(std::cin, username);
    std::cout << "Create password: ";
    std::string password;
    std::getline(std::cin, password);
    auto user = userManager->registerUser(username, password);
    if (user) {
        // Collect profile information
        std::cout << "Gender (M/F): ";
        std::string gender;
        std::getline(std::cin, gender);
        std::cout << "Height (in cm): ";
        std::string heightStr;
        std::getline(std::cin, heightStr);
        double height = std::stod(heightStr);
        std::cout << "Age: ";
        std::string ageStr;
        std::getline(std::cin, ageStr);
        int age = std::stoi(ageStr);
        std::cout << "Weight (in kg): ";
        std::string weightStr;
        std::getline(std::cin, weightStr);
        double weight = std::stod(weightStr);
        std::cout << "Activity Level:" << std::endl;
        std::cout << "1. Sedentary" << std::endl;
        std::cout << "2. Lightly Active" << std::endl;
        std::cout << "3. Moderately Active" << std::endl;
        std::cout << "4. Very Active" << std::endl;
        std::cout << "5. Extremely Active" << std::endl;
        std::cout << "Choose an activity level (1-5): ";
        std::string activityChoiceStr;
        std::getline(std::cin, activityChoiceStr);
        int activityChoice = std::stoi(activityChoiceStr);
        // ActivityLevel enum is zero-indexed
        user->getDietProfile()->setGender(gender);
        user->getDietProfile()->setHeight(height);
        user->getDietProfile()->setAge(age);
        user->getDietProfile()->updateDailyStats(currentDate, age, weight, static_cast<ActivityLevel>(activityChoice - 1));
        user->getDietProfile()->setCalorieCalculator(std::make_shared<HarrisBenedictCalculator>());
        currentUser = user;
        std::cout << "Registration successful. Welcome, " << username << "!" << std::endl;
        return true;
    } else {
        std::cout << "Registration failed. Username may already be taken." << std::endl;
        return false;
    }
}

// Add this method to the YADAApplication class
void YADAApplication::updateFoodInLog() {
    std::cout << "\n--- Update Food in Log ---" << std::endl;
    auto dailyLog = logManager->getLogForDate(currentDate);
    if (!dailyLog || dailyLog->getEntries().empty()) {
        std::cout << "No food entries for this date." << std::endl;
        return;
    }
    
    // Display existing entries
    auto entries = dailyLog->getEntries();
    for (auto &entry : entries) {
        std::cout << "ID: " << entry->getId() << " - " << entry->getServings() 
                  << " servings of " << entry->getFood()->getId() << " - " 
                  << entry->getCalories() << " calories" << std::endl;
    }
    
    // Ask for entry ID
    std::cout << "Enter the ID of the entry to update (0 to cancel): ";
    std::string idStr;
    std::getline(std::cin, idStr);
    int entryId = std::stoi(idStr);
    if (entryId == 0) {
        std::cout << "Update canceled." << std::endl;
        return;
    }
    
    // Check if the entry exists
    auto entry = dailyLog->findEntryById(entryId);
    if (!entry) {
        std::cout << "No entry with that ID exists for this date." << std::endl;
        return;
    }
    
    // Ask for new servings
    std::cout << "Current servings: " << entry->getServings() << std::endl;
    std::cout << "Enter new servings amount: ";
    std::string servingsStr;
    std::getline(std::cin, servingsStr);
    double newServings = std::stod(servingsStr);
    
    // Create and execute command
    auto command = std::make_shared<UpdateFoodCommand>(logManager.get(), currentDate, entryId, newServings);
    commandProcessor->executeCommand(command);
    std::cout << "Food entry updated successfully." << std::endl;
}

void YADAApplication::displayFoodManager() {
    std::cout << "\n--- YADA Food Management ---" << std::endl;
    std::cout << "Current Date: " << currentDate << std::endl;
    std::cout << "1. View Food Database" << std::endl;
    std::cout << "2. Add Basic Food to Database" << std::endl;
    std::cout << "3. Create Composite Food" << std::endl;
    std::cout << "4. Back to Main Menu" << std::endl;

    std::cout << "Choose an option: ";
}

void YADAApplication::displayDailyUpdateMenu() {
    std::cout << "\n--- YADA Daily ---" << std::endl;
    std::cout << "1. Add Food to Daily Log" << std::endl;
    std::cout << "2. View Daily Log" << std::endl;
    std::cout << "3. Remove Food from Daily Log" << std::endl;
    std::cout << "4. Update Food in Daily Log" << std::endl;
    std::cout << "5. View Diet Statistics" << std::endl;
    std::cout << "6. Back to Main Menu" << std::endl;

    std::cout << "Choose an option: ";
}

void YADAApplication::displayProfileView() {
    std::cout << "\n--- YADA Profile ---" << std::endl;
    std::cout << "1. Update Daily Stats" << std::endl;
    std::cout << "2. Change Calorie Calculator Method" << std::endl;
    std::cout << "3. View Profile" << std::endl;
    std::cout << "4. Back to Main Menu" << std::endl;
    std::cout << "Choose an option: ";
}

void YADAApplication::displayMenu() {
    std::cout << "\n--- YADA Menu ---" << std::endl;
    std::cout << "Current Date: " << currentDate << std::endl;
    std::cout << "1. Food Management" << std::endl;
    std::cout << "2. Daily Updates" << std::endl;
    std::cout << "3. Manage Your Profile " << std::endl;
    std::cout << "4. Change Date" << std::endl;
    std::cout << "5. Undo Last Action" << std::endl;
    std::cout << "6. Save Data" << std::endl;
    std::cout << "7. Exit" << std::endl;
    std::cout << "Choose an option: ";
}
void YADAApplication::displayUserProfile() {
    auto profile = currentUser->getDietProfile();
    auto currentStats = profile->getDailyStats(currentDate);

    std::cout << "\n--- User Profile ---" << std::endl;
    std::cout << "Username: " << currentUser->getUsername() << std::endl;
    std::cout << "Gender: " << profile->getGender() << std::endl;
    std::cout << "Height: " << profile->getHeight() << " cm" << std::endl;
    
    // Show current age and weight from daily stats
    if (currentStats) {
        std::cout << "Current Age: " << currentStats->getAge() << " years" << std::endl;
        std::cout << "Current Weight: " << currentStats->getWeight() << " kg" << std::endl;
        std::cout << "Current Activity Level: ";
        switch(currentStats->getActivityLevel()) {
            case 0:
                std::cout << "Sedentary";
                break;
            case 1:
                std::cout << "Lightly Active";
                break;
            case 2:
                std::cout << "Moderately Active";
                break;
            case 3:
                std::cout << "Very Active";
                break;
            case 4:
                std::cout << "Extremely Active";
                break;
        }
        std::cout << std::endl;
    } else {
        std::cout << "No current stats available for " << currentDate << std::endl;
    }

    // Show calculator type
    std::cout << "Calorie Calculator Method: ";
    if (profile->getCalorieCalculator()->getType() == "HarrisBenedict") {
        std::cout << "Harris-Benedict Equation";
    } else {
        std::cout << "Mifflin-St Jeor Equation";
    }
    std::cout << std::endl;

    // Show target and consumed calories if stats exist
    if (currentStats) {
        double targetCalories = profile->getTargetCalories(currentDate);
        double consumedCalories = 0.0;
        auto dailyLog = logManager->getLogForDate(currentDate);
        if (dailyLog) {
            consumedCalories = dailyLog->getTotalCalories();
        }
        std::cout << "\nDaily Calorie Summary for " << currentDate << ":" << std::endl;
        std::cout << "Target Calories: " << targetCalories << std::endl;
        std::cout << "Consumed Calories: " << consumedCalories << std::endl;
        double difference = targetCalories - consumedCalories;
        if (difference > 0) {
            std::cout << "Remaining Calories: " << difference << std::endl;
        } else {
            std::cout << "Calories Over Target: " << -difference << std::endl;
        }
    }
}

bool YADAApplication::processCommandProfile(const std::string &choice) {
    try {
        if (choice == "1") {
            updateDailyStats();
        } else if (choice == "2") {
            changeCalorieCalculator();
        } else if (choice == "3") {
            displayUserProfile();
        } else if (choice == "4") {
            return true;
        } else {
            std::cout << "Invalid option. Please try again." << std::endl;
        }
    } catch (const std::exception &e) {
        std::cout << "Error processing command: " << e.what() << std::endl;
    }
    return true;
}


bool YADAApplication::processCommandFood(const std::string &choice) {
    try {
        if (choice == "1") {
            viewFoodDatabase();
        } else if (choice == "2") {
            addBasicFood();
        } else if (choice == "3") {
            createCompositeFood();
        } else if (choice == "4") {
            return true;
        } else {
            std::cout << "Invalid option. Please try again." << std::endl;
        }
    } catch (const std::exception &e) {
        std::cout << "Error processing command: " << e.what() << std::endl;
    }
    return true;
}

bool YADAApplication::processCommandDaily(const std::string &choice) {
    try {
        if (choice == "1") {
            addFoodToLog();
        } else if (choice == "2") {
            viewDailyLog();
        } else if (choice == "3") {
            removeFoodFromLog();
        } else if (choice == "4") {
            updateFoodInLog();
        } else if (choice == "5") {
            viewDietStatistics();
        } else if (choice == "6") {
            return true;
        } else {
            std::cout << "Invalid option. Please try again." << std::endl;
        }
    } catch (const std::exception &e) {
        std::cout << "Error processing command: " << e.what() << std::endl;
    }
    return true;
}

bool YADAApplication::processCommand(const std::string &choice) {
    try {
        if (choice == "4") {
            changeCurrentDate();
        } else if (choice == "5") {
            undoLastAction();
        } else if (choice == "6") {
            saveAll();
        } else if (choice == "7") {
            return false;
        } else {
            std::cout << "Invalid option. Please try again." << std::endl;
        }
    } catch (const std::exception &e) {
        std::cout << "Error processing command: " << e.what() << std::endl;
    }
    return true;
}

void YADAApplication::viewFoodDatabase() {
    std::cout << "\n--- Food Database ---" << std::endl;
     
    // Get search input from user
    std::cout << "Enter search term (or press Enter to see all foods): ";
    std::string searchTerm;
    std::getline(std::cin, searchTerm);

    auto foods = foodDatabase->getAllFoods();
    if (foods.empty()) {
        std::cout << "No foods in the database." << std::endl;
        return;
    }
    std::cout << "Basic Foods:" << std::endl;
    bool foundBasic = false;
    for (auto &food : foods) {
        if (auto basicFood = dynamic_cast<BasicFood*>(food.get())) {
            bool matches = searchTerm.empty();  // Show all if no search term
            if (!matches) {
                // Case-insensitive search in ID
                std::string foodId = food->getId();
                std::transform(foodId.begin(), foodId.end(), foodId.begin(), ::tolower);
                std::string searchLower = searchTerm;
                std::transform(searchLower.begin(), searchLower.end(), searchLower.begin(), ::tolower);
                
                if (foodId.find(searchLower) != std::string::npos) {
                    matches = true;
                } else {
                    // Search in keywords
                    auto keywords = food->getKeywords();
                    for (const auto &kw : keywords) {
                        std::string kwLower = kw;
                        std::transform(kwLower.begin(), kwLower.end(), kwLower.begin(), ::tolower);
                        if (kwLower.find(searchLower) != std::string::npos) {
                            matches = true;
                            break;
                        }
                    }
                }
            }

            if (matches) {
                foundBasic = true;
                std::cout << food->getId() << " - " << food->getCalories() << " calories per serving" << std::endl;
                std::cout << "  Keywords: ";
                auto kws = food->getKeywords();
                for (size_t i = 0; i < kws.size(); ++i) {
                    std::cout << kws[i] << (i < kws.size() - 1 ? ", " : "");
                }
                std::cout << std::endl;
            }
        }
    }
    if (!foundBasic) {
        std::cout << "No matching basic foods found." << std::endl;
    }

    std::cout << "\nComposite Foods:" << std::endl;
    bool foundComposite = false;
    for (auto &food : foods) {
        if (auto compositeFood = dynamic_cast<CompositeFood*>(food.get())) {
            bool matches = searchTerm.empty();  // Show all if no search term
            if (!matches) {
                // Case-insensitive search in ID
                std::string foodId = food->getId();
                std::transform(foodId.begin(), foodId.end(), foodId.begin(), ::tolower);
                std::string searchLower = searchTerm;
                std::transform(searchLower.begin(), searchLower.end(), searchLower.begin(), ::tolower);
                
                if (foodId.find(searchLower) != std::string::npos) {
                    matches = true;
                } else {
                    // Search in keywords
                    auto keywords = food->getKeywords();
                    for (const auto &kw : keywords) {
                        std::string kwLower = kw;
                        std::transform(kwLower.begin(), kwLower.end(), kwLower.begin(), ::tolower);
                        if (kwLower.find(searchLower) != std::string::npos) {
                            matches = true;
                            break;
                        }
                    }
                }
            }

            if (matches) {
                foundComposite = true;
                std::cout << food->getId() << " - " << food->getCalories() << " calories per serving" << std::endl;
                std::cout << "  Keywords: ";
                auto kws = food->getKeywords();
                for (size_t i = 0; i < kws.size(); ++i) {
                    std::cout << kws[i] << (i < kws.size() - 1 ? ", " : "");
                }
                std::cout << std::endl;
                std::cout << "  Components:" << std::endl;
                for (auto &entry : compositeFood->getComponents()) {
                    std::cout << "    " << entry.second << " servings of " << entry.first->getId() << std::endl;
                }
            }

            // std::cout << food->getId() << " - " << food->getCalories() << " calories per serving" << std::endl;
            // std::cout << "  Keywords: ";
            // auto kws = food->getKeywords();
            // for (size_t i = 0; i < kws.size(); ++i) {
            //     std::cout << kws[i] << (i < kws.size() - 1 ? ", " : "");
            // }
            // std::cout << std::endl;
            // CompositeFood* comp = dynamic_cast<CompositeFood*>(food.get());
            // std::cout << "  Components:" << std::endl;
            // for (auto &entry : comp->getComponents()) {
            //     std::cout << "    " << entry.second << " servings of " << entry.first->getId() << std::endl;
            // }
        }
    }
    if (!foundComposite) {
        std::cout << "No matching composite foods found." << std::endl;
    }

    if (!foundBasic && !foundComposite) {
        std::cout << "\nNo foods found matching '" << searchTerm << "'" << std::endl;
    }
}

void YADAApplication::addBasicFood() {
    std::cout << "\n--- Add Basic Food ---" << std::endl;
    std::cout << "Food identifier: ";
    std::string id;
    std::getline(std::cin, id);
    if (foodDatabase->findFood(id) != nullptr) {
        std::cout << "A food with this identifier already exists." << std::endl;
        return;
    }
    std::cout << "Keywords (comma separated): ";
    std::string keywordsInput;
    std::getline(std::cin, keywordsInput);
    std::vector<std::string> keywords;
    std::istringstream iss(keywordsInput);
    std::string token;
    while (std::getline(iss, token, ',')) {
        token.erase(std::remove_if(token.begin(), token.end(), ::isspace), token.end());
        if (!token.empty())
            keywords.push_back(token);
    }
    std::cout << "Calories per serving: ";
    std::string caloriesStr;
    std::getline(std::cin, caloriesStr);
    double calories = std::stod(caloriesStr);
    auto basic = std::make_shared<BasicFood>(id, keywords, calories);
    foodDatabase->addFood(basic);

    // Use command pattern for adding food
    auto command = std::make_shared<AddFoodToDatabaseCommand>(foodDatabase.get(), basic);
    commandProcessor->executeCommand(command);

    std::cout << "Basic food added successfully!" << std::endl;
}

void YADAApplication::createCompositeFood() {
    std::cout << "\n--- Create Composite Food ---" << std::endl;
    std::cout << "Composite food identifier: ";
    std::string id;
    std::getline(std::cin, id);
    if (foodDatabase->findFood(id) != nullptr) {
        std::cout << "A food with this identifier already exists." << std::endl;
        return;
    }
    std::cout << "Keywords (comma separated): ";
    std::string keywordsInput;
    std::getline(std::cin, keywordsInput);
    std::vector<std::string> keywords;
    std::istringstream iss(keywordsInput);
    std::string token;
    while (std::getline(iss, token, ',')) {
        token.erase(std::remove_if(token.begin(), token.end(), ::isspace), token.end());
        if (!token.empty())
            keywords.push_back(token);
    }
    auto composite = std::make_shared<CompositeFood>(id, keywords);
    bool addingComponents = true;
    while (addingComponents) {
        std::cout << "\nSearch for a component food:" << std::endl;
        std::cout << "Enter search keyword (comma separated) or 'list' to see all foods: ";
        std::string searchInput;
        std::getline(std::cin, searchInput);
        std::vector<std::shared_ptr<FoodItem>> results;
        if (searchInput == "list") {
            results = foodDatabase->getAllFoods();
        } else {
            std::string searchLower = searchInput;
            std::transform(searchLower.begin(), searchLower.end(), searchLower.begin(), ::tolower);

            // Get all foods and filter based on partial matches
            auto allFoods = foodDatabase->getAllFoods();
            // std::vector<std::string> searchKeywords;
            // std::istringstream ss(searchInput);
            for (const auto& food : allFoods) {
                bool matches = false;
                
                // Check food identifier for partial match
                std::string foodId = food->getId();
                std::transform(foodId.begin(), foodId.end(), foodId.begin(), ::tolower);
                if (foodId.find(searchLower) != std::string::npos) {
                    matches = true;
                }
                
                // Check food keywords for partial matches
                if (!matches) {
                    for (const auto& keyword : food->getKeywords()) {
                        std::string keywordLower = keyword;
                        std::transform(keywordLower.begin(), keywordLower.end(), keywordLower.begin(), ::tolower);
                        if (keywordLower.find(searchLower) != std::string::npos) {
                            matches = true;
                            break;
                        }
                    }
                }
    
                if (matches) {
                    results.push_back(food);
                }
            }
        }
        if (results.empty()) {
            std::cout << "No foods found matching those keywords." << std::endl;
            continue;
        }
        std::cout << "Found foods:" << std::endl;
        for (size_t i = 0; i < results.size(); ++i) {
            std::string dummy = "";
            for (size_t j = 0; j < results[i]->getKeywords().size() - 1; ++j) {
                dummy += results[i]->getKeywords()[j] + ", ";
            }
            dummy += results[i]->getKeywords()[results[i]->getKeywords().size() - 1];
            std::cout << (i + 1) << ". " << results[i]->getId() << "[" << dummy << "]"<< " - " << results[i]->getCalories() << " calories per serving" << std::endl;
            // I want to add the ingredients in case of a composite food
            if (auto compositeFood = dynamic_cast<CompositeFood*>(results[i].get())) {
                std::cout << "    Components:" << std::endl;
                for (auto &entry : compositeFood->getComponents()) {
                    std::cout << "     " << entry.second << " servings of " << entry.first->getId() << std::endl;
                }
            }
        }
        std::cout << "Select a food by number (0 to cancel): ";
        std::string selStr;
        std::getline(std::cin, selStr);
        int selection = std::stoi(selStr);
        if (selection == 0) {
            std::cout << "Selection canceled." << std::endl;
        } else if (selection > 0 && selection <= static_cast<int>(results.size())) {
            auto selectedFood = results[selection - 1];
            std::cout << "Number of servings: ";
            std::string servingsStr;
            std::getline(std::cin, servingsStr);
            double servings = std::stod(servingsStr);
            composite->addComponent(selectedFood, servings);
            std::cout << "Component added!" << std::endl;
        } else {
            std::cout << "Invalid selection." << std::endl;
        }
        std::cout << "Add another component? (y/n): ";
        std::string another;
        std::getline(std::cin, another);
        addingComponents = (another == "y" || another == "Y");
    }
    if (composite->getComponents().empty()) {
        std::cout << "Composite food creation canceled - no components added." << std::endl;
        return;
    }
    foodDatabase->addFood(composite);
    auto command = std::make_shared<AddFoodToDatabaseCommand>(foodDatabase.get(), composite);
    commandProcessor->executeCommand(command);

    std::cout << "Composite food created successfully!" << std::endl;
    std::cout << "Total calories per serving: " << composite->getCalories() << std::endl;
}

void YADAApplication::addFoodToLog() {
    std::cout << "\n--- Add Food to Log ---" << std::endl;
    auto selectedFood = selectFood();
    if (!selectedFood)
        return;
    std::cout << "Number of servings: ";
    std::string servingsStr;
    std::getline(std::cin, servingsStr);
    double servings = std::stod(servingsStr);
    auto command = std::make_shared<AddFoodCommand>(logManager.get(), currentDate, selectedFood, servings);
    commandProcessor->executeCommand(command);
    std::cout << "Food added to log for " << currentDate << std::endl;
}

std::shared_ptr<FoodItem> YADAApplication::selectFood() {
    std::cout << "Enter search keywords (comma separated) or 'list' to see all foods: ";
    std::string searchInput;
    std::getline(std::cin, searchInput);
    std::vector<std::shared_ptr<FoodItem>> results;
    if (searchInput == "list") {
        results = foodDatabase->getAllFoods();
    } else {
        std::vector<std::string> searchKeywords;
        std::istringstream ss(searchInput);
        std::string token;
        while (std::getline(ss, token, ',')) {
            token.erase(std::remove_if(token.begin(), token.end(), ::isspace), token.end());
            if (!token.empty())
                searchKeywords.push_back(token);
        }
        std::cout << "Match all keywords? (y/n): ";
        std::string matchAllStr;
        std::getline(std::cin, matchAllStr);
        bool matchAll = (matchAllStr == "y" || matchAllStr == "Y");
        results = foodDatabase->searchFoods(searchKeywords, matchAll);
    }
    if (results.empty()) {
        std::cout << "No foods found matching those keywords." << std::endl;
        return nullptr;
    }
    std::cout << "Found foods:" << std::endl;
    for (size_t i = 0; i < results.size(); ++i) {
        std::cout << (i + 1) << ". " << results[i]->getId() << " - " << results[i]->getCalories() << " calories per serving" << std::endl;
    }
    std::cout << "Select a food by number (0 to cancel): ";
    std::string selStr;
    std::getline(std::cin, selStr);
    int selection = std::stoi(selStr);
    if (selection == 0) {
        std::cout << "Selection canceled." << std::endl;
        return nullptr;
    } else if (selection > 0 && selection <= static_cast<int>(results.size())) {
        return results[selection - 1];
    } else {
        std::cout << "Invalid selection." << std::endl;
        return nullptr;
    }
}

void YADAApplication::viewDailyLog() {
    std::cout << "\n--- Daily Log for " << currentDate << " ---" << std::endl;
    auto dailyLog = logManager->getLogForDate(currentDate);
    if (!dailyLog || dailyLog->getEntries().empty()) {
        std::cout << "No food entries for this date." << std::endl;
        return;
    }
    auto entries = dailyLog->getEntries();
    for (auto &entry : entries) {
        std::cout << "ID: " << entry->getId() << " - " << entry->getServings() 
                  << " servings of " << entry->getFood()->getId() << " - " 
                  << entry->getCalories() << " calories" << std::endl;
    }
    std::cout << "\nTotal calories: " << dailyLog->getTotalCalories() << std::endl;
}

void YADAApplication::removeFoodFromLog() {
    std::cout << "\n--- Remove Food from Log ---" << std::endl;
    auto dailyLog = logManager->getLogForDate(currentDate);
    if (!dailyLog || dailyLog->getEntries().empty()) {
        std::cout << "No food entries for this date." << std::endl;
        return;
    }
    auto entries = dailyLog->getEntries();
    for (auto &entry : entries) {
        std::cout << "ID: " << entry->getId() << " - " << entry->getServings() 
                  << " servings of " << entry->getFood()->getId() << " - " 
                  << entry->getCalories() << " calories" << std::endl;
    }
    std::cout << "Enter the ID of the entry to remove (0 to cancel): ";
    std::string idStr;
    std::getline(std::cin, idStr);
    int entryId = std::stoi(idStr);
    if (entryId == 0) {
        std::cout << "Removal canceled." << std::endl;
        return;
    }
    bool exists = false;
    for (auto &entry : entries) {
        if (entry->getId() == entryId) {
            exists = true;
            break;
        }
    }
    if (!exists) {
        std::cout << "No entry with that ID exists for this date." << std::endl;
        return;
    }
    auto command = std::make_shared<RemoveFoodCommand>(logManager.get(), currentDate, entryId);
    commandProcessor->executeCommand(command);
    std::cout << "Food entry removed from log." << std::endl;
}

void YADAApplication::viewDietStatistics() {
    std::cout << "\n--- Diet Statistics for " << currentDate << " ---" << std::endl;
    auto profile = currentUser->getDietProfile();
    auto stats = profile->getDailyStats(currentDate);
    if (!stats) {
        std::cout << "No statistics available for this date. Please update your daily stats." << std::endl;
        return;
    }
    double targetCalories = profile->getTargetCalories(currentDate);
    double consumedCalories = 0.0;
    auto dailyLog = logManager->getLogForDate(currentDate);
    if (dailyLog)
        consumedCalories = dailyLog->getTotalCalories();
    double difference = consumedCalories - targetCalories;
    std::cout << "Weight: " << stats->getWeight() << " kg" << std::endl;
    std::cout << "Activity Level: " << static_cast<int>(stats->getActivityLevel() )<< std::endl;
    std::cout << "Target Calories: " << targetCalories << std::endl;
    std::cout << "Consumed Calories: " << consumedCalories << std::endl;
    if (difference < 0)
        std::cout << "Calories Available: " << -difference << std::endl;
    else
        std::cout << "Calories Over Target: " << difference << std::endl;
    // Show which calculator is being used
    if (profile->getCalorieCalculator()->getType() == "HarrisBenedict")
        std::cout << "Using Harris-Benedict equation for calorie calculation" << std::endl;
    else if (profile->getCalorieCalculator()->getType() == "MifflinStJeor")
        std::cout << "Using Mifflin-St Jeor equation for calorie calculation" << std::endl;
}

// void YADAApplication::updateDailyStats() {
//     std::cout << "\n--- Update Daily Stats for " << currentDate << " ---" << std::endl;
//     auto profile = currentUser->getDietProfile();
//     auto currentStats = profile->getDailyStats(currentDate);
//     std::cout << "Weight (in kg)" 
//               << (currentStats ? " [current: " + std::to_string(currentStats->getWeight()) + "]" : "") 
//               << ": ";
//     std::string weightInput;
//     std::getline(std::cin, weightInput);
//     double weight;
//     if (weightInput.empty() && currentStats)
//         weight = currentStats->getWeight();
//     else
//         weight = std::stod(weightInput);

//     std::cout << "Age (in years)" 
//                 << (currentStats ? " [current: " + std::to_string(currentStats->getAge()) + "]" : "") 
//                 << ": ";
//     std::string ageInput;
//     // possibly empty input
//     // if empty, use current stats
//     std::getline(std::cin, ageInput);
//     int age;
//     if (ageInput.empty() && currentStats)
//         age = currentStats->getAge();
//     else
//         age = std::stod(ageInput);
    

//     std::cout << "Activity Level:" << std::endl;
//     std::cout << "1. Sedentary" << std::endl;
//     std::cout << "2. Lightly Active" << std::endl;
//     std::cout << "3. Moderately Active" << std::endl;
//     std::cout << "4. Very Active" << std::endl;
//     std::cout << "5. Extremely Active" << std::endl;
//     int currentActivity = currentStats ? static_cast<int>(currentStats->getActivityLevel()) : 0;
//     std::cout << "Choose an activity level (1-5)"
//               << " [current: " << (currentActivity + 1) << "]: ";
//     std::string activityInput;
//     std::getline(std::cin, activityInput);
//     int activityChoice = currentActivity;
//     if (!activityInput.empty())
//         activityChoice = std::stoi(activityInput) - 1;
//     profile->updateDailyStats(currentDate, weight, age, static_cast<ActivityLevel>(activityChoice));
//     std::cout << "Daily stats updated successfully!" << std::endl;
// }

void YADAApplication::updateDailyStats() {
    std::cout << "\n--- Update Daily Stats for " << currentDate << " ---" << std::endl;
    auto profile = currentUser->getDietProfile();
    auto currentStats = profile->getDailyStats(currentDate);
    
    // Prompt for age update
    std::cout << "Age" 
              << (currentStats ? " [current: " + std::to_string(currentStats->getAge()) + "]" : 
                  " [profile: " + std::to_string(profile->getAge()) + "]") 
              << ": ";
    std::string ageInput;
    std::getline(std::cin, ageInput);
    int age;
    if (ageInput.empty() && currentStats)
        age = currentStats->getAge();
    else if (ageInput.empty())
        age = profile->getAge();
    else
        age = std::stoi(ageInput);
    
    // Prompt for weight update
    std::cout << "Weight (in kg)" 
              << (currentStats ? " [current: " + std::to_string(currentStats->getWeight()) + "]" : "") 
              << ": ";
    std::string weightInput;
    std::getline(std::cin, weightInput);
    double weight;
    if (weightInput.empty() && currentStats)
        weight = currentStats->getWeight();
    else
        weight = std::stod(weightInput);
    
    // Prompt for activity level update
    std::cout << "Activity Level:" << std::endl;
    std::cout << "1. Sedentary" << std::endl;
    std::cout << "2. Lightly Active" << std::endl;
    std::cout << "3. Moderately Active" << std::endl;
    std::cout << "4. Very Active" << std::endl;
    std::cout << "5. Extremely Active" << std::endl;
    
    int currentActivity = currentStats ? static_cast<int>(currentStats->getActivityLevel()) : 0;
    std::cout << "Choose an activity level (1-5)"
              << (currentStats ? " [current: " + std::to_string(currentActivity + 1) + "]" : "") 
              << ": ";
    std::string activityInput;
    std::getline(std::cin, activityInput);
    int activityChoice = currentActivity;
    if (!activityInput.empty())
        activityChoice = std::stoi(activityInput) - 1;
    
    // Create and execute the command for updating profile
    auto command = std::make_shared<UpdateProfileCommand>(
        profile.get(), currentDate, age, weight, static_cast<ActivityLevel>(activityChoice));
    commandProcessor->executeCommand(command);
    
    std::cout << "Daily stats updated successfully!" << std::endl;
}

void YADAApplication::changeCalorieCalculator() {
    std::cout << "\n--- Change Calorie Calculator Method ---" << std::endl;
    std::cout << "Available methods:" << std::endl;
    std::cout << "1. Harris-Benedict Equation" << std::endl;
    std::cout << "2. Mifflin-St Jeor Equation" << std::endl;
    std::cout << "Choose a method: ";
    std::string choice;
    std::getline(std::cin, choice);
    // if (choice == "1") {
    //     currentUser->getDietProfile()->setCalorieCalculator(std::make_shared<HarrisBenedictCalculator>());
    //     std::cout << "Changed to Harris-Benedict Equation." << std::endl;
    // } else if (choice == "2") {
    //     currentUser->getDietProfile()->setCalorieCalculator(std::make_shared<MifflinStJeorCalculator>());
    //     std::cout << "Changed to Mifflin-St Jeor Equation." << std::endl;
    // } else {
    //     std::cout << "Invalid choice. No changes made." << std::endl;
    // }
    std::shared_ptr<CalorieCalculator> newCalculator;
    if (choice == "1") {
        newCalculator = std::make_shared<HarrisBenedictCalculator>();
        auto command = std::make_shared<ChangeCalorieCalculatorCommand>(
            currentUser->getDietProfile().get(), newCalculator);
        commandProcessor->executeCommand(command);
        std::cout << "Changed to Harris-Benedict Equation." << std::endl;
    } else if (choice == "2") {
        newCalculator = std::make_shared<MifflinStJeorCalculator>();
        auto command = std::make_shared<ChangeCalorieCalculatorCommand>(
            currentUser->getDietProfile().get(), newCalculator);
        commandProcessor->executeCommand(command);
        std::cout << "Changed to Mifflin-St Jeor Equation." << std::endl;
    } else {
        std::cout << "Invalid choice. No changes made." << std::endl;
    }
}

void YADAApplication::changeCurrentDate() {
    std::cout << "\n--- Change Current Date ---" << std::endl;
    std::cout << "Current date: " << currentDate << std::endl;
    std::cout << "Enter new date (yyyy-MM-dd): ";
    std::string dateStr;
    std::getline(std::cin, dateStr);
    // For simplicity we assume the date format is correct.
    currentDate = dateStr;
    std::cout << "Date changed to: " << currentDate << std::endl;
}

void YADAApplication::undoLastAction() {
    if (commandProcessor->undo()) {
        std::cout << "Last action undone successfully." << std::endl;
    } else {
        std::cout << "Nothing to undo." << std::endl;
    }
}
