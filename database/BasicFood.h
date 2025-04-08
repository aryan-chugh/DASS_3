#ifndef BASICFOOD_H
#define BASICFOOD_H

#include "FoodItem.h"

class BasicFood : public FoodItem {
public:
    BasicFood(const std::string &id, const std::vector<std::string> &keywords, double caloriesPerServing);
    double getCalories() const override;
    void setCalories(double calories);
private:
    double caloriesPerServing;
};

#endif // BASICFOOD_H
