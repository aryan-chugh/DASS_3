// ChangeCalorieCalculatorCommand.cpp
#include "ChangeCalorieCalculatorCommand.h"

ChangeCalorieCalculatorCommand::ChangeCalorieCalculatorCommand(
    DietProfile* profile, std::shared_ptr<CalorieCalculator> newCalculator)
    : profile(profile), newCalculator(newCalculator) {
    
    // Store the old calculator for undo capability
    oldCalculator = profile->getCalorieCalculator();
}

void ChangeCalorieCalculatorCommand::execute() {
    profile->setCalorieCalculator(newCalculator);
}

void ChangeCalorieCalculatorCommand::undo() {
    profile->setCalorieCalculator(oldCalculator);
}