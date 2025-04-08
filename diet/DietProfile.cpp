#include "DietProfile.h"
#include <algorithm>

DietProfile::DietProfile() : gender(""), height(0), age(0), calorieCalculator(nullptr) {}

std::string DietProfile::getGender() const {
    return gender;
}

void DietProfile::setGender(const std::string &gender) {
    this->gender = gender;
}

double DietProfile::getHeight() const {
    return height;
}

void DietProfile::setHeight(double height) {
    this->height = height;
}

int DietProfile::getAge() const {
    return age;
}

void DietProfile::setAge(int age) {
    this->age = age;
}

void DietProfile::updateDailyStats(const std::string &date, int age, double weight, int activityLevel) {
    dailyStats[date] = std::make_shared<DailyStats>(weight, age, static_cast<ActivityLevel>(activityLevel));
}

std::shared_ptr<DailyStats> DietProfile::getDailyStats(const std::string &date) const {
    if (dailyStats.find(date) != dailyStats.end())
        return dailyStats.at(date);
    // Find most recent previous date if available (simplified by lexicographical order)
    std::string recent = "";
    for (const auto &pair : dailyStats) {
        if (pair.first < date && (recent.empty() || pair.first > recent))
            recent = pair.first;
    }
    return !recent.empty() ? dailyStats.at(recent) : nullptr;
}

std::map<std::string, std::shared_ptr<DailyStats>> DietProfile::getAllDailyStats() const {
    return dailyStats;
}

void DietProfile::setCalorieCalculator(const std::shared_ptr<CalorieCalculator> &calculator) {
    calorieCalculator = calculator;
}

std::shared_ptr<CalorieCalculator> DietProfile::getCalorieCalculator() const {
    return calorieCalculator;
}
double DietProfile::getTargetCalories(const std::string &date) const {
    auto stats = getDailyStats(date);
    if (!stats || !calorieCalculator)
        return 0;
    
    // Use the daily stats age instead of the profile age
    return calorieCalculator->calculateTargetCalories(gender, stats->getAge(), height, stats->getWeight(), stats->getActivityLevel());
}