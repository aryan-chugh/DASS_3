#include "CommandProcessor.h"

void CommandProcessor::executeCommand(const std::shared_ptr<Command> &command) {
    command->execute();
    undoStack.push(command);
}

bool CommandProcessor::undo() {
    if (undoStack.empty())
        return false;
    auto command = undoStack.top();
    undoStack.pop();
    command->undo();
    return true;
}
