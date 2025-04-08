// AddFoodCommand.h
#ifndef ADDFOODCOMMAND_H
#define ADDFOODCOMMAND_H

#include "../command/Command.h"
#include "../Logging/LogManager.h"
#include <memory>

class AddFoodCommand : public Command {
private:
    LogManager* logManager;
    std::string date;
    std::shared_ptr<FoodItem> food;
    double servings;
    LogUpdateResult result;

public:
    AddFoodCommand(LogManager* lm, const std::string& d, std::shared_ptr<FoodItem> f, double s)
        : logManager(lm), date(d), food(f), servings(s) {}

    void execute() override {
        result = logManager->addFoodToLog(date, food, servings);
    }

    void undo() override {
        if (result.isNewEntry) {
            // If it was a new entry, remove it entirely
            logManager->removeFoodFromLog(date, result.entryId);
        } else {
            // If it was an existing entry, subtract the servings we added
            logManager->updateFoodInLog(date, result.entryId, result.originalServings);
        }
    }
};

#endif // ADDFOODCOMMAND_H