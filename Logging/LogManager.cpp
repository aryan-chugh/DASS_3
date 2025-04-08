// LogManager.cpp (Updated - no filesystem dependency)
#include "LogManager.h"
#include <stdexcept>
#include <iostream>
#include <fstream>

#ifdef _WIN32
    #include <direct.h>
    #define MKDIR(dir) _mkdir(dir)
#else
    #include <sys/stat.h>
    #define MKDIR(dir) mkdir(dir, 0777)
#endif

LogManager::LogManager(const std::string& username) 
    : username(username) {
    foodLog = std::make_unique<FoodLog>();
    createDirectories();
}

void LogManager::createDirectories() {
    // Create logs directory if it doesn't exist
    MKDIR("logs");
    
    // Create user-specific directory
    std::string userDir = "logs/" + username;
    MKDIR(userDir.c_str());
}

std::string LogManager::getLogFilePath() const {
    return "logs/" + username + "/" + LOG_FILE;
}

LogUpdateResult LogManager::addFoodToLog(const std::string &date, const std::shared_ptr<FoodItem> &food, double servings) {
    auto dailyLog = foodLog->getLogForDate(date);
    LogUpdateResult result{-1, 0.0, true};

    if (dailyLog) {
        // Check if food already exists in today's log
        for (const auto& existingEntry : dailyLog->getEntries()) {
            if (existingEntry->getFood()->getId() == food->getId()) {
                // Update servings of existing entry
                result.entryId = existingEntry->getId();
                result.originalServings = existingEntry->getServings();
                result.isNewEntry = false;
                
                double newServings = existingEntry->getServings() + servings;
                existingEntry->setServings(newServings);
                return result;
            }
        }
    }
    
    // If not found or no daily log exists, create new entry
    auto entry = std::make_shared<FoodLogEntry>(food, servings);
    result.entryId = entry->getId();
    result.originalServings = 0.0;
    foodLog->addEntry(date, entry);
    return result;
}

std::shared_ptr<FoodLogEntry> LogManager::removeFoodFromLog(const std::string &date, int entryId) {
    return foodLog->removeEntry(date, entryId);
}

double LogManager::updateFoodInLog(const std::string &date, int entryId, double newServings) {
    auto dailyLog = foodLog->getLogForDate(date);
    if (!dailyLog) return -1.0;
    
    auto entry = dailyLog->findEntryById(entryId);
    if (!entry) return -1.0;
    
    double originalServings = entry->getServings();
    foodLog->updateEntry(date, entryId, newServings);
    return originalServings;
}

std::shared_ptr<DailyLog> LogManager::getLogForDate(const std::string &date) {
    return foodLog->getLogForDate(date);
}

void LogManager::save() {
    foodLog->saveLog(getLogFilePath());
}

void LogManager::load(FoodDatabase* foodDatabase) {
    foodLog->loadLog(getLogFilePath(), foodDatabase);
}

void LogManager::restoreEntry(const std::string& date, std::shared_ptr<FoodLogEntry> entry) {
    if (!entry) return;
    foodLog->addEntry(date, entry); // Will use the existing entry ID
}
