// // AddFoodCommand.cpp
// #include "AddFoodCommand.h"

// AddFoodCommand::AddFoodCommand(LogManager* logManager, const std::string& date, 
//                                const std::shared_ptr<FoodItem>& food, double servings)
//     : logManager(logManager), date(date), food(food), servings(servings), entryId(-1) {}

// void AddFoodCommand::execute() {
//     entryId = logManager->addFoodToLog(date, food, servings);
// }

// void AddFoodCommand::undo() {
//     if (entryId != -1) {
//         logManager->removeFoodFromLog(date, entryId);
//     }
// }