// DailyLog.cpp (Updated)
#include "DailyLog.h"
#include <algorithm>

DailyLog::DailyLog(const std::string &date)
    : date(date) {}

void DailyLog::addEntry(const std::shared_ptr<FoodLogEntry> &entry) {
    // Check if there's already an entry with the same food item
    std::string foodId = entry->getFood()->getId();
    auto existingEntry = findEntryByFoodId(foodId);
    
    if (existingEntry) {
        // Update the existing entry's servings
        double newServings = existingEntry->getServings() + entry->getServings();
        existingEntry->setServings(newServings);
    } else {
        // Add as a new entry
        entries.push_back(entry);
    }
}

std::shared_ptr<FoodLogEntry> DailyLog::removeEntry(int entryId) {
    std::shared_ptr<FoodLogEntry> removedEntry = nullptr;
    
    for (auto it = entries.begin(); it != entries.end(); ++it) {
        if ((*it)->getId() == entryId) {
            removedEntry = *it;
            entries.erase(it);
            break;
        }
    }
    
    return removedEntry;
}

bool DailyLog::updateEntry(int entryId, double newServings) {
    for (auto &entry : entries) {
        if (entry->getId() == entryId) {
            entry->setServings(newServings);
            return true;
        }
    }
    return false;
}

std::vector<std::shared_ptr<FoodLogEntry>> DailyLog::getEntries() const {
    return entries;
}

double DailyLog::getTotalCalories() const {
    double total = 0;
    for (auto &entry : entries)
        total += entry->getCalories();
    return total;
}

std::shared_ptr<FoodLogEntry> DailyLog::findEntryById(int entryId) const {
    for (auto &entry : entries)
        if (entry->getId() == entryId)
            return entry;
    return nullptr;
}

std::shared_ptr<FoodLogEntry> DailyLog::findEntryByFoodId(const std::string &foodId) const {
    for (auto &entry : entries)
        if (entry->getFood()->getId() == foodId)
            return entry;
    return nullptr;
}