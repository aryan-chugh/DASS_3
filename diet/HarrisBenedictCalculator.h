#ifndef HARRISBENEDICTCALCULATOR_H
#define HARRISBENEDICTCALCULATOR_H

#include "CalorieCalculator.h"

class HarrisBenedictCalculator : public CalorieCalculator {
public:
    double calculateTargetCalories(const std::string &gender, int age, double height, double weight, ActivityLevel activity) const override;
    std::string getType() const override;
};

#endif // HARRISBENEDICTCALCULATOR_H
