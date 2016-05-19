#pragma once

#include <cmath>
#include <deque>
#include <vector>

#include "s3ePointer.h"

struct Event
{
    bool motion;
    uint32 id;
    uint32 pressed;
    int32 x;
    int32 y;
};

struct Touch
{
    Touch() {
        pressed = false;
        id = 0;
        x = y = 0;
        prevX = prevY = 0;
        origX = origY = 0;
    }

    void reset() {
        pressed = false;
        id = 0;
        x = y = 0;
        prevX = prevY = 0;
        origX = origY = 0;
    }

    float dist() {
        int32 dx = origX - x;
        int32 dy = origY - y;

        return sqrtf(dx*dx + dy*dy);
    }

    bool pressed;
    uint32 id;
    int32 x;
    int32 y;
    int32 prevX;
    int32 prevY;
    int32 origX;
    int32 origY;
};

// http://www.drmop.com/index.php/2011/09/24/marmalade-sdk-tutorial-touch-and-multi-touch/
class Input
{
public:

    bool hasEvents();
    Event popEvent();

public:

    void update();
    void reset();

    void unregister();

    static Input& getInstance();

    static void SingleTouchButtonCB(s3ePointerEvent* event);
    static void SingleTouchMotionCB(s3ePointerMotionEvent* event);
    static void MultiTouchButtonCB(s3ePointerTouchEvent* event);
    static void MultiTouchMotionCB(s3ePointerTouchMotionEvent* event);

private:

    Input();

    void pushEvent(Event e);

    bool isMultiTouch;
    std::deque<Event> events;

    static Input* instance;
};