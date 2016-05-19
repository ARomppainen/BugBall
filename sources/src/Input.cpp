#include "Input.h"

Input* Input::instance = 0;

Input::Input()
{
    isMultiTouch = s3ePointerGetInt(S3E_POINTER_MULTI_TOUCH_AVAILABLE) ? true : false;

    // set touch event callback handler methods
    if (isMultiTouch) {
        s3ePointerRegister(S3E_POINTER_TOUCH_EVENT, (s3eCallback)MultiTouchButtonCB, 0);
        s3ePointerRegister(S3E_POINTER_TOUCH_MOTION_EVENT, (s3eCallback)MultiTouchMotionCB, 0);
    } else {
        s3ePointerRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)SingleTouchButtonCB, 0);
        s3ePointerRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)SingleTouchMotionCB, 0);
    }
}

void Input::unregister()
{
    if (isMultiTouch) {
        s3ePointerUnRegister(S3E_POINTER_TOUCH_EVENT, (s3eCallback)MultiTouchButtonCB);
        s3ePointerUnRegister(S3E_POINTER_TOUCH_MOTION_EVENT, (s3eCallback)MultiTouchMotionCB);
    } else {
        s3ePointerUnRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)SingleTouchButtonCB);
        s3ePointerUnRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)SingleTouchMotionCB);
    }
}

void Input::update()
{
    s3ePointerUpdate();
}

Input& Input::getInstance()
{
    if ( !instance ) {
        instance = new Input();
    }

    return *instance;
}

bool Input::hasEvents()
{
    return !events.empty();
}

Event Input::popEvent()
{
    Event ret = events.front();
    events.pop_front();
    return ret;
}

void Input::pushEvent(Event e)
{
    events.push_back(e);
}

void Input::SingleTouchButtonCB(s3ePointerEvent* event)
{
    Event e;
    e.motion = false;
    e.id = 0;
    e.pressed = event->m_Pressed;
    e.x = event->m_x;
    e.y = event->m_y;

    instance->pushEvent(e);
}

void Input::SingleTouchMotionCB(s3ePointerMotionEvent* event)
{
    Event e;
    e.motion = true;
    e.id = 0;
    e.pressed = 0;
    e.x = event->m_x;
    e.y = event->m_y;

    instance->pushEvent(e);
}

void Input::MultiTouchButtonCB(s3ePointerTouchEvent* event)
{
    Event e;
    e.motion = false;
    e.id = event->m_TouchID;
    e.pressed = event->m_Pressed;
    e.x = event->m_x;
    e.y = event->m_y;

    instance->pushEvent(e);
}

void Input::MultiTouchMotionCB(s3ePointerTouchMotionEvent* event)
{
    Event e;
    e.motion = true;
    e.id = event->m_TouchID;
    e.pressed = 0;
    e.x = event->m_x;
    e.y = event->m_y;

    instance->pushEvent(e);
}