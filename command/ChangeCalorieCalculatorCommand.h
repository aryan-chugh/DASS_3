// ChangeCalorieCalculatorCommand.h
#ifndef CHANGECALORIECALCULATORCOMMAND_H
#define CHANGECALORIECALCULATORCOMMAND_H

#include "../command/Command.h"
#include "../diet/DietProfile.h"
#include "../diet/CalorieCalculator.h"
#include <memory>

class ChangeCalorieCalculatorCommand : public Command {
public:
    ChangeCalorieCalculatorCommand(DietProfile* profile, std::shared_ptr<CalorieCalculator> newCalculator);
    void execute() override;
    void undo() override;

private:
    DietProfile* profile;
    std::shared_ptr<CalorieCalculator> newCalculator;
    std::shared_ptr<CalorieCalculator> oldCalculator;
};

#endif // CHANGECALORIECALCULATORCOMMAND_H