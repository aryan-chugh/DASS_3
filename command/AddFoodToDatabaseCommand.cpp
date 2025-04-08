#include "AddFoodToDatabaseCommand.h"

AddFoodToDatabaseCommand::AddFoodToDatabaseCommand(FoodDatabase* database, const std::shared_ptr<FoodItem>& food)
    : database(database), food(food), executed(false) {}

void AddFoodToDatabaseCommand::execute() {
    if (!executed) {
        database->addFood(food);
        executed = true;
    }
}

void AddFoodToDatabaseCommand::undo() {
    if (executed) {
        database->removeFood(food->getId());
        executed = false;
    }
}