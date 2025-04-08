#include "FoodDatabase.h"
#include "BasicFood.h"
#include "CompositeFood.h"
#include <fstream>
#include <sstream>
#include <iostream>

#include <algorithm>
#include <iterator>

FoodDatabase::FoodDatabase() {}

void FoodDatabase::addFood(const std::shared_ptr<FoodItem> &food) {
    foods[food->getId()] = food;
}

void FoodDatabase::removeFood(const std::string &id) {
    foods.erase(id);
}

std::shared_ptr<FoodItem> FoodDatabase::findFood(const std::string &id) {
    return foods.count(id) ? foods[id] : nullptr;
}

std::vector<std::shared_ptr<FoodItem>> FoodDatabase::searchFoods(const std::vector<std::string> &keywords, bool matchAll) {
    std::vector<std::shared_ptr<FoodItem>> results;
    std::vector<std::string> lowerKeywords;
    for (const auto &kw : keywords) {
        std::string lower = kw;
        std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
        lowerKeywords.push_back(lower);
    }

    for (const auto &pair : foods) {
        bool matches = false;
        
        // Convert food ID to lowercase for comparison
        std::string foodId = pair.second->getId();
        std::transform(foodId.begin(), foodId.end(), foodId.begin(), ::tolower);

        auto foodKeywords = pair.second->getKeywords();
        std::vector<std::string> lowerFoodKeywords;
        for (const auto &fkw : foodKeywords) {
            std::string lower = fkw;
            std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
            lowerFoodKeywords.push_back(lower);
        }

        if (matchAll) {
            bool allMatch = true;
            for (const auto &searchKw : lowerKeywords) {
                // Check if keyword matches food ID
                if (foodId.find(searchKw) != std::string::npos) {
                    allMatch = true;
                }else {
                    allMatch = false;
                    break;
                }

               // Check if keyword matches any food keyword
                bool found = false;
                for (const auto &foodKw : lowerFoodKeywords) {
                    if (foodKw.find(searchKw) != std::string::npos) {
                        found = true;
                        break;
                    }
                }
                if(found) {
                    allMatch = true;
                } else {
                    allMatch = false;
                    break;
                }
            }
            if (allMatch)
                results.push_back(pair.second);
        } else {
            for (const auto &searchKw : lowerKeywords) {
                // Check if keyword matches food ID
                if (foodId.find(searchKw) != std::string::npos) {
                    matches = true;
                    break;
                }
                
                // Check if keyword matches any food keyword
                for (const auto &foodKw : lowerFoodKeywords) {
                    if (foodKw.find(searchKw) != std::string::npos) {
                        matches = true;
                        break;
                    }
                }
            }
            if (matches) {
                results.push_back(pair.second);
            }
        }
    nextFood:
        continue;
    }
    return results;
}

std::vector<std::shared_ptr<FoodItem>> FoodDatabase::getAllFoods() {
    std::vector<std::shared_ptr<FoodItem>> all;
    for (const auto &pair : foods)
        all.push_back(pair.second);
    return all;
}

void FoodDatabase::saveDatabase() {
    std::ofstream basicOut(BASIC_FOOD_FILE);
    for (const auto &pair : foods) {
        if (dynamic_cast<BasicFood*>(pair.second.get())) {
            basicOut << "BASIC|" << pair.second->getId() << "|";
            auto kws = pair.second->getKeywords();
            for (size_t i = 0; i < kws.size(); ++i) {
                basicOut << kws[i] << (i < kws.size()-1 ? "," : "");
            }
            basicOut << "|" << pair.second->getCalories() << "\n";
        }
    }
    basicOut.close();
    std::ofstream compositeOut(COMPOSITE_FOOD_FILE);
    for (const auto &pair : foods) {
        if (auto comp = dynamic_cast<CompositeFood*>(pair.second.get())) {
            compositeOut << "COMPOSITE|" << comp->getId() << "|";
            auto kws = comp->getKeywords();
            for (size_t i = 0; i < kws.size(); ++i) {
                compositeOut << kws[i] << (i < kws.size()-1 ? "," : "");
            }
            compositeOut << "|";
            bool first = true;
            for (auto &entry : comp->getComponents()) {
                if (!first) compositeOut << ",";
                compositeOut << entry.first->getId() << ":" << entry.second;
                first = false;
            }
            compositeOut << "\n";
        }
    }
}

void FoodDatabase::loadDatabase() {
    foods.clear();
    loadBasicFoods();
    loadCompositeFoods();
}

void FoodDatabase::loadBasicFoods() {
    std::ifstream ifs(BASIC_FOOD_FILE);
    if (!ifs)
        return;
    std::string line;
    while (std::getline(ifs, line)) {
        if (line.empty()) continue;
        std::istringstream iss(line);
        std::string type, id, keywordsStr, caloriesStr;
        std::getline(iss, type, '|');
        std::getline(iss, id, '|');
        std::getline(iss, keywordsStr, '|');
        std::getline(iss, caloriesStr, '|');
        std::vector<std::string> kws;
        std::istringstream kwStream(keywordsStr);
        std::string kw;
        while (std::getline(kwStream, kw, ','))
            kws.push_back(kw);
        double calories = std::stod(caloriesStr);
        auto basic = std::make_shared<BasicFood>(id, kws, calories);
        foods[id] = basic;
    }
}

void FoodDatabase::loadCompositeFoods() {
    std::ifstream ifs(COMPOSITE_FOOD_FILE);
    if (!ifs)
        return;
    std::string line;
    while (std::getline(ifs, line)) {
        if (line.empty()) continue;
        std::istringstream iss(line);
        std::string type, id, keywordsStr, componentsStr;
        std::getline(iss, type, '|');
        std::getline(iss, id, '|');
        std::getline(iss, keywordsStr, '|');
        std::getline(iss, componentsStr, '|');
        std::vector<std::string> kws;
        std::istringstream kwStream(keywordsStr);
        std::string kw;
        while (std::getline(kwStream, kw, ','))
            kws.push_back(kw);
        auto composite = std::make_shared<CompositeFood>(id, kws);
        std::istringstream compStream(componentsStr);
        std::string compPair;
        while (std::getline(compStream, compPair, ',')) {
            if (!compPair.empty()) {
                auto pos = compPair.find(':');
                if (pos != std::string::npos) {
                    std::string compId = compPair.substr(0, pos);
                    double servings = std::stod(compPair.substr(pos+1));
                    auto compFood = findFood(compId);
                    if (compFood)
                        composite->addComponent(compFood, servings);
                }
            }
        }
        foods[id] = composite;
    }
}
