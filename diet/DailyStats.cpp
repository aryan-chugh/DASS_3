#include "DailyStats.h"

DailyStats::DailyStats(double weight, int age, ActivityLevel activityLevel)
    : weight(weight), age(age), activityLevel(activityLevel) {}

double DailyStats::getWeight() const {
    return weight;
}

ActivityLevel DailyStats::getActivityLevel() const {
    return activityLevel;
}
