// LogManager.h (Updated)
#ifndef LOGMANAGER_H
#define LOGMANAGER_H

#include <string>
#include <memory>
#include "FoodLog.h"
#include "../database/FoodItem.h"
#include "../database/FoodDatabase.h"

struct LogUpdateResult {
    int entryId;
    double originalServings;
    bool isNewEntry;
};

class LogManager {
private:
    std::string username;
    std::unique_ptr<FoodLog> foodLog;
    const std::string LOG_FILE = "food_log.txt";
    
    // Helper methods for file operations
    void createDirectories();
    std::string getLogFilePath() const;

public:
    LogManager(const std::string& username = "default");
    
    // Returns the new entry ID
    // int addFoodToLog(const std::string &date, const std::shared_ptr<FoodItem> &food, double servings);
    
    // Returns the removed entry (to support undo)
    std::shared_ptr<FoodLogEntry> removeFoodFromLog(const std::string &date, int entryId);
    
    // Update entry servings (returns original servings value for undo)
    double updateFoodInLog(const std::string &date, int entryId, double newServings);
    
    std::shared_ptr<DailyLog> getLogForDate(const std::string &date);
    void save();
    void load(FoodDatabase* foodDatabase);

    LogUpdateResult addFoodToLog(const std::string &date, const std::shared_ptr<FoodItem> &food, double servings);
    
    // Add a new method to restore entry with original ID
    void restoreEntry(const std::string& date, std::shared_ptr<FoodLogEntry> entry);
};

#endif // LOGMANAGER_H
