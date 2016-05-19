#pragma once

// rendering layers from front to back
enum Layer
{
    FOREGROUND       = 1,
    UI               = 2,
    CHARACTER_FRONT  = 3,
    CHARACTER        = 4,
    CHARACTER_BEHIND = 5,
    BOARD            = 6,
    WATER            = 7,
    BACKGROUND       = 8
};