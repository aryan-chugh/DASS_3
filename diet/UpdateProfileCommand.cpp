// UpdateProfileCommand.cpp
#include "UpdateProfileCommand.h"

UpdateProfileCommand::UpdateProfileCommand(DietProfile* profile, const std::string& date, 
                                           int age, double weight, ActivityLevel activityLevel)
    : profile(profile), date(date), newAge(age), newWeight(weight), 
      newActivityLevel(activityLevel), hadPreviousStats(false) {
    
    // Store previous values for undo capability
    auto previousStats = profile->getDailyStats(date);
    if (previousStats) {
        oldAge = previousStats->getAge();
        oldWeight = previousStats->getWeight();
        oldActivityLevel = previousStats->getActivityLevel();
        hadPreviousStats = true;
    }
}

void UpdateProfileCommand::execute() {
    profile->updateDailyStats(date, newAge, newWeight, static_cast<int>(newActivityLevel));
}

void UpdateProfileCommand::undo() {
    if (hadPreviousStats) {
        profile->updateDailyStats(date, oldAge, oldWeight, static_cast<int>(oldActivityLevel));
    } else {
        // If there were no previous stats, we would need to remove the entry
        // However, the current API doesn't have a method to remove daily stats
        // For simplicity, we'll leave this as a limitation
    }
}