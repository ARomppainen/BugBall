#include "Button.h"

Button::Button()
{
    command = NULL;
}

Button::~Button()
{
    if (command != NULL) {
        delete command;
        command = NULL;
    }
}

void Button::click()
{
    if (command != NULL) {
        command->execute();
    }
}

void Button::setCommand(Command* c)
{
    this->command = c;
}