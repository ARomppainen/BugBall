#pragma once

#include "Character.h"

class Ball : public Character
{
public:
	Ball();
	Ball(int movement);
	virtual ~Ball();

    virtual void updateOffset();
};

