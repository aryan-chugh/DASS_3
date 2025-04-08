#ifndef COMPOSITEFOOD_H
#define COMPOSITEFOOD_H

#include "FoodItem.h"
#include <map>
#include <memory>

class CompositeFood : public FoodItem {
public:
    CompositeFood(const std::string &id, const std::vector<std::string> &keywords);
    void addComponent(const std::shared_ptr<FoodItem> &food, double servings);
    void removeComponent(const std::string &foodId);
    double getCalories() const override;
    std::map<std::shared_ptr<FoodItem>, double> getComponents() const;
private:
    std::map<std::shared_ptr<FoodItem>, double> components;
};

#endif // COMPOSITEFOOD_H
