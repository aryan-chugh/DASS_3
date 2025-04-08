#ifndef COMMANDPROCESSOR_H
#define COMMANDPROCESSOR_H

#include <stack>
#include <memory>
#include "Command.h"

class CommandProcessor {
public:
    void executeCommand(const std::shared_ptr<Command> &command);
    bool undo();
private:
    std::stack<std::shared_ptr<Command>> undoStack;
};

#endif // COMMANDPROCESSOR_H
