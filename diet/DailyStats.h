#ifndef DAILYSTATS_H
#define DAILYSTATS_H

#include "ActivityLevel.h"

class DailyStats {
public:
    
    DailyStats() : weight(0), age(0), activityLevel(ActivityLevel::SEDENTARY) {}
    DailyStats(double weight, int age, ActivityLevel activityLevel);
    double getWeight() const;
    ActivityLevel getActivityLevel() const;
    void setAge(int age) { this->age = age; }
    int getAge() const { return age; }

    void setWeight(double wt) { this->weight= wt; }
    void setActivityLevel(ActivityLevel activityLevel) { this->activityLevel = activityLevel; }
private:
    double weight;
    ActivityLevel activityLevel;
    int age;
};

#endif // DAILYSTATS_H
