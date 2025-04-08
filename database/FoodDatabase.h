#ifndef FOODDATABASE_H
#define FOODDATABASE_H

#include <string>
#include <map>
#include <vector>
#include <memory>
#include "FoodItem.h"

class FoodDatabase {
public:
    FoodDatabase();
    void addFood(const std::shared_ptr<FoodItem> &food);
    void removeFood(const std::string &id);
    std::shared_ptr<FoodItem> findFood(const std::string &id);
    std::vector<std::shared_ptr<FoodItem>> searchFoods(const std::vector<std::string> &keywords, bool matchAll);
    std::vector<std::shared_ptr<FoodItem>> getAllFoods();
    void saveDatabase();
    void loadDatabase();
private:
    std::map<std::string, std::shared_ptr<FoodItem>> foods;
    const std::string BASIC_FOOD_FILE = "basic_foods.txt";
    const std::string COMPOSITE_FOOD_FILE = "composite_foods.txt";
    void loadBasicFoods();
    void loadCompositeFoods();
};

#endif // FOODDATABASE_H
