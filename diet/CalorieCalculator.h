#ifndef CALORIECALCULATOR_H
#define CALORIECALCULATOR_H

#include "ActivityLevel.h"
#include <string>

class CalorieCalculator {
public:
    virtual ~CalorieCalculator() {}
    virtual double calculateTargetCalories(const std::string &gender, int age, double height, double weight, ActivityLevel activity) const = 0;
    virtual std::string getType() const = 0;
};

#endif // CALORIECALCULATOR_H
