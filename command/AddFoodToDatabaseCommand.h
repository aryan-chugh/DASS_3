#ifndef ADD_FOOD_TO_DATABASE_COMMAND_H
#define ADD_FOOD_TO_DATABASE_COMMAND_H

#include "../command/Command.h"
#include "../database/FoodDatabase.h"
#include "../database/FoodItem.h"
#include <memory>

/**
 * Command for adding a food item to the database
 * Supports undo operation to remove the added food item
 */
class AddFoodToDatabaseCommand : public Command {
private:
    FoodDatabase* database;
    std::shared_ptr<FoodItem> food;
    bool executed;

public:
    /**
     * Constructor
     * @param database Pointer to the food database
     * @param food Food item to add to the database
     */
    AddFoodToDatabaseCommand(FoodDatabase* database, const std::shared_ptr<FoodItem>& food);
    
    /**
     * Execute the command - add food to database
     */
    void execute() override;
    
    /**
     * Undo the command - remove food from database
     */
    void undo() override;
};

#endif // ADD_FOOD_TO_DATABASE_COMMAND_H