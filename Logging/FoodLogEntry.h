// FoodLogEntry.h (Updated)
#ifndef FOODLOGENTRY_H
#define FOODLOGENTRY_H

#include <string>
#include <memory>
#include <ctime>
#include "../database/FoodItem.h"

class FoodLogEntry {
public:
    FoodLogEntry(const std::shared_ptr<FoodItem> &food, double servings);
    FoodLogEntry(int id, const std::shared_ptr<FoodItem> &food, double servings, const std::string& timestamp);
    int getId() const;
    std::shared_ptr<FoodItem> getFood() const;
    double getServings() const;
    void setServings(double servings);
    std::string getTimestamp() const;
    double getCalories() const;
    
    // Static method to reset ID counter (useful for testing or reloading)
    static void resetIdCounter(int value = 0) { nextId = value; }
    
private:
    static int nextId;
    int id;
    std::shared_ptr<FoodItem> food;
    double servings;
    std::string timestamp; // Using time_t for timestamp
};

#endif // FOODLOGENTRY_H