#ifndef DIETPROFILE_H
#define DIETPROFILE_H

#include <string>
#include <map>
#include <memory>
#include "DailyStats.h"
#include "CalorieCalculator.h"

class DietProfile {
public:
    DietProfile();
    std::string getGender() const;
    void setGender(const std::string &gender);
    double getHeight() const;
    void setHeight(double height);
    int getAge() const;
    void setAge(int age);
    void updateDailyStats(const std::string &date, int age, double weight, int activityLevel);
    std::shared_ptr<DailyStats> getDailyStats(const std::string &date) const;
    std::map<std::string, std::shared_ptr<DailyStats>> getAllDailyStats() const;
    void setCalorieCalculator(const std::shared_ptr<CalorieCalculator> &calculator);
    std::shared_ptr<CalorieCalculator> getCalorieCalculator() const;
    double getTargetCalories(const std::string &date) const;
private:
    std::string gender;
    double height;
    int age;
    std::map<std::string, std::shared_ptr<DailyStats>> dailyStats;
    std::shared_ptr<CalorieCalculator> calorieCalculator;
};

#endif // DIETPROFILE_H
