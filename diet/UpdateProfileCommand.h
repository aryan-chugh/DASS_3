// UpdateProfileCommand.h
#ifndef UPDATEPROFILECOMMAND_H
#define UPDATEPROFILECOMMAND_H

#include "../command/Command.h"
#include "../diet/DietProfile.h"
#include <memory>
#include <string>

class UpdateProfileCommand : public Command {
public:
    UpdateProfileCommand(DietProfile* profile, const std::string& date, 
                         int age, double weight, ActivityLevel activityLevel);
    void execute() override;
    void undo() override;

private:
    DietProfile* profile;
    std::string date;
    int newAge;
    double newWeight;
    ActivityLevel newActivityLevel;
    int oldAge;
    double oldWeight;
    ActivityLevel oldActivityLevel;
    bool hadPreviousStats;
};

#endif // UPDATEPROFILECOMMAND_H