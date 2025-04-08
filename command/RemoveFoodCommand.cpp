// // RemoveFoodCommand.cpp
// #include "RemoveFoodCommand.h"

// RemoveFoodCommand::RemoveFoodCommand(LogManager* logManager, const std::string& date, int entryId)
//     : logManager(logManager), date(date), entryId(entryId), removedEntry(nullptr) {}

// void RemoveFoodCommand::execute() {
//     removedEntry = logManager->removeFoodFromLog(date, entryId);
// }

// void RemoveFoodCommand::undo() {
//     if (removedEntry) {
//         logManager->restoreEntry(date, removedEntry);
//     }
// }