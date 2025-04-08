// DailyLog.h (Updated)
#ifndef DAILYLOG_H
#define DAILYLOG_H

#include <string>
#include <vector>
#include <memory>
#include "FoodLogEntry.h"

class DailyLog {
public:
    DailyLog(const std::string &date);
    
    // Add entry - now handles duplicate food items
    void addEntry(const std::shared_ptr<FoodLogEntry> &entry);
    
    // Updated removeEntry to return the removed entry
    std::shared_ptr<FoodLogEntry> removeEntry(int entryId);
    
    // Update an existing entry (modify servings)
    bool updateEntry(int entryId, double newServings);
    
    std::vector<std::shared_ptr<FoodLogEntry>> getEntries() const;
    double getTotalCalories() const;
    std::shared_ptr<FoodLogEntry> findEntryById(int entryId) const;
    std::shared_ptr<FoodLogEntry> findEntryByFoodId(const std::string &foodId) const;
    const std::string& getDate() const { return date; }
    
private:
    std::string date;
    std::vector<std::shared_ptr<FoodLogEntry>> entries;
};

#endif // DAILYLOG_H