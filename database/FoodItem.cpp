#include "FoodItem.h"

FoodItem::FoodItem(const std::string &id, const std::vector<std::string> &keywords)
    : id(id), keywords(keywords) {}

std::string FoodItem::getId() const {
    return id;
}

std::vector<std::string> FoodItem::getKeywords() const {
    return keywords;
}
