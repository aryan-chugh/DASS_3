#include "HarrisBenedictCalculator.h"

double HarrisBenedictCalculator::calculateTargetCalories(const std::string &gender, int age, double height, double weight, ActivityLevel activity) const {
    double bmr;
    if (gender == "M" || gender == "m") {
        bmr = 88.362 + (13.397 * weight) + (4.799 * height) - (5.677 * age);
    } else {
        bmr = 447.593 + (9.247 * weight) + (3.098 * height) - (4.330 * age);
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

std::string HarrisBenedictCalculator::getType() const {
    return "HarrisBenedict";
}
