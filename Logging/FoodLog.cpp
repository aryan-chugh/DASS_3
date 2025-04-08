
// FoodLog.cpp (Updated)
#include "FoodLog.h"
#include "FoodLogEntry.h"
#include "../database/FoodDatabase.h"
#include <fstream>
#include <sstream>
#include <iostream>

void FoodLog::addEntry(const std::string &date, const std::shared_ptr<FoodLogEntry> &entry) {
    if (logs.find(date) == logs.end())
        logs[date] = std::make_shared<DailyLog>(date);
    logs[date]->addEntry(entry);
}

std::shared_ptr<FoodLogEntry> FoodLog::removeEntry(const std::string &date, int entryId) {
    if (logs.find(date) != logs.end())
        return logs[date]->removeEntry(entryId);
    return nullptr;
}

bool FoodLog::updateEntry(const std::string &date, int entryId, double newServings) {
    if (logs.find(date) != logs.end())
        return logs[date]->updateEntry(entryId, newServings);
    return false;
}

std::shared_ptr<DailyLog> FoodLog::getLogForDate(const std::string &date) {
    return logs.count(date) ? logs[date] : nullptr;
}

double FoodLog::getTotalCaloriesForDate(const std::string &date) const {
    if (logs.count(date))
        return logs.at(date)->getTotalCalories();
    return 0;
}

void FoodLog::saveLog(const std::string &filename) const {
    std::ofstream ofs(filename);
    for (const auto &pair : logs) {
        const std::string &date = pair.first;
        for (auto &entry : pair.second->getEntries()) {
            ofs << date << "|" << entry->getId() << "|" << entry->getFood()->getId() << "|" 
                << entry->getServings() << "|" << entry->getTimestamp() << "\n";
        }
    }
}

void FoodLog::loadLog(const std::string &filename, FoodDatabase *foodDatabase) {
    logs.clear();
    std::ifstream ifs(filename);
    if (!ifs)
        return;
    std::string line;
    while (std::getline(ifs, line)) {
        if (line.empty()) continue;
        std::istringstream iss(line);
        std::string date, idStr, foodId, servingsStr, timestamp;
        std::getline(iss, date, '|');
        std::getline(iss, idStr, '|');
        std::getline(iss, foodId, '|');
        std::getline(iss, servingsStr, '|');
        std::getline(iss, timestamp, '|');
        int id = std::stoi(idStr);
        double servings = std::stod(servingsStr);
        auto food = foodDatabase->findFood(foodId);
        if (food) {
            auto entry = std::make_shared<FoodLogEntry>(id, food, servings, timestamp);
            addEntry(date, entry);
        }
    }
}