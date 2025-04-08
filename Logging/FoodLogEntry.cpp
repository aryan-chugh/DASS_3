// FoodLogEntry.cpp (Updated)
#include "FoodLogEntry.h"
#include <ctime>
#include <sstream>

int FoodLogEntry::nextId = 1;

FoodLogEntry::FoodLogEntry(const std::shared_ptr<FoodItem> &food, double servings)
    : id(nextId++), food(food), servings(servings) {
    // Simple timestamp generation
    std::time_t t = std::time(nullptr);
    timestamp = std::to_string(t);
}

// nextId is incremented in the constructor to ensure unique IDs
// and to avoid the need for a separate static method to generate IDs.
FoodLogEntry::FoodLogEntry(int id, const std::shared_ptr<FoodItem> &food, double servings, const std::string &timestamp)
    : id(id), food(food), servings(servings), timestamp(timestamp) {
    if (id >= nextId)
        nextId = id + 1;
}

int FoodLogEntry::getId() const {
    return id;
}

std::shared_ptr<FoodItem> FoodLogEntry::getFood() const {
    return food;
}

double FoodLogEntry::getServings() const {
    return servings;
}

void FoodLogEntry::setServings(double servings) {
    this->servings = servings;
}

std::string FoodLogEntry::getTimestamp() const {
    return timestamp;
}

double FoodLogEntry::getCalories() const {
    return food->getCalories() * servings;
}