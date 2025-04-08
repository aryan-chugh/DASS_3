// UpdateFoodCommand.h
#ifndef UPDATEFOODCOMMAND_H
#define UPDATEFOODCOMMAND_H

#include "../command/Command.h"
#include "../Logging/LogManager.h"

class UpdateFoodCommand : public Command {
private:
    LogManager* logManager;
    std::string date;
    int entryId;
    double newServings;
    double oldServings;
    bool isExecuted;

public:
    UpdateFoodCommand(LogManager* logManager, const std::string& date, 
                     int entryId, double newServings)
        : logManager(logManager), date(date), entryId(entryId),
          newServings(newServings), oldServings(-1), isExecuted(false) {}

    void execute() override {
        oldServings = logManager->updateFoodInLog(date, entryId, newServings);
        isExecuted = oldServings >= 0;
    }

    void undo() override {
        if (isExecuted && oldServings >= 0) {
            logManager->updateFoodInLog(date, entryId, oldServings);
            isExecuted = false;
        }
    }
};

#endif // UPDATEFOODCOMMAND_H