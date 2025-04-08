#include "CompositeFood.h"

CompositeFood::CompositeFood(const std::string &id, const std::vector<std::string> &keywords)
    : FoodItem(id, keywords) {}

void CompositeFood::addComponent(const std::shared_ptr<FoodItem> &food, double servings) {
    components[food] = servings;
}

void CompositeFood::removeComponent(const std::string &foodId) {
    for (auto it = components.begin(); it != components.end(); ) {
        if (it->first->getId() == foodId)
            it = components.erase(it);
        else
            ++it;
    }
}

double CompositeFood::getCalories() const {
    double total = 0;
    for (const auto &entry : components) {
        total += entry.first->getCalories() * entry.second;
    }
    return total;
}

std::map<std::shared_ptr<FoodItem>, double> CompositeFood::getComponents() const {
    return components;
}
