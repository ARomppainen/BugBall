#pragma once

#include "Command.h"
#include "Drawable.h"

class Button : public Drawable
{
public:
    Button();
    ~Button();

    void click();
    void setCommand(Command* c);

private:

    Command* command;

    //Texture* pressed;
    //Texture* notPressed;
};