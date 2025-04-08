#include "MifflinStJeorCalculator.h"

double MifflinStJeorCalculator::calculateTargetCalories(const std::string &gender, int age, double height, double weight, ActivityLevel activity) const {
    double bmr;
    if (gender == "M" || gender == "m") {
        bmr = (10 * weight) + (6.25 * height) - (5 * age) + 5;
    } else {
        bmr = (10 * weight) + (6.25 * height) - (5 * age) - 161;
    }
    double multiplier;
    switch(activity) {
        case SEDENTARY: multiplier = 1.2; break;
        case LIGHTLY_ACTIVE: multiplier = 1.375; break;
        case MODERATELY_ACTIVE: multiplier = 1.55; break;
        case VERY_ACTIVE: multiplier = 1.725; break;
        case EXTREMELY_ACTIVE: multiplier = 1.9; break;
        default: multiplier = 1.2; break;
    }
    return bmr * multiplier;
}

std::string MifflinStJeorCalculator::getType() const {
    return "MifflinStJeor";
}
