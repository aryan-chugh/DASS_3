#ifndef MIFFLINSTJEORCALCULATOR_H
#define MIFFLINSTJEORCALCULATOR_H

#include "CalorieCalculator.h"

class MifflinStJeorCalculator : public CalorieCalculator {
public:
    double calculateTargetCalories(const std::string &gender, int age, double height, double weight, ActivityLevel activity) const override;
    std::string getType() const override;
};

#endif // MIFFLINSTJEORCALCULATOR_H
