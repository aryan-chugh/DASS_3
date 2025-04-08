// RemoveFoodCommand.h
#ifndef REMOVEFOODCOMMAND_H
#define REMOVEFOODCOMMAND_H

#include "../command/Command.h"
#include "../Logging/LogManager.h"
#include <memory>

class RemoveFoodCommand : public Command {
private:
    LogManager* logManager;
    std::string date;
    int entryId;
    std::shared_ptr<FoodLogEntry> removedEntry; // Store the entire entry

public:
    RemoveFoodCommand(LogManager* lm, const std::string& d, int id)
        : logManager(lm), date(d), entryId(id) {}

    void execute() override {
        removedEntry = logManager->removeFoodFromLog(date, entryId);
    }

    void undo() override {
        if (removedEntry) {
            logManager->restoreEntry(date, removedEntry); // Restore with original ID
        }
    }
};

#endif // REMOVEFOODCOMMAND_H