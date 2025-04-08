// FoodLog.h (Updated)
#ifndef FOODLOG_H
#define FOODLOG_H

#include <string>
#include <map>
#include <memory>
#include "DailyLog.h"

class FoodLog {
public:
    void addEntry(const std::string &date, const std::shared_ptr<FoodLogEntry> &entry);
    std::shared_ptr<FoodLogEntry> removeEntry(const std::string &date, int entryId);
    bool updateEntry(const std::string &date, int entryId, double newServings);
    
    std::shared_ptr<DailyLog> getLogForDate(const std::string &date);
    double getTotalCaloriesForDate(const std::string &date) const;
    void saveLog(const std::string &filename) const;
    void loadLog(const std::string &filename, class FoodDatabase *foodDatabase);
    std::map<std::string, std::shared_ptr<DailyLog>> getAllLogs() const { return logs; }
    void clear() { logs.clear(); }
    
private:
    std::map<std::string, std::shared_ptr<DailyLog>> logs;
};

#endif // FOODLOG_H