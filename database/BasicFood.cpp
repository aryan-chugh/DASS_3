#include "BasicFood.h"

BasicFood::BasicFood(const std::string &id, const std::vector<std::string> &keywords, double caloriesPerServing)
    : FoodItem(id, keywords), caloriesPerServing(caloriesPerServing) {}

double BasicFood::getCalories() const {
    return caloriesPerServing;
}

void BasicFood::setCalories(double calories) {
    caloriesPerServing = calories;
}
