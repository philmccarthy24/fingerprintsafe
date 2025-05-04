#include <Arduino.h>
#include "MicroSwitch.h"

MicroSwitch::MicroSwitch(uint8_t buttonPin) :
    m_buttonPin(buttonPin),
    m_lastButtonState(HIGH)
{
}

MicroSwitch::~MicroSwitch()
{
}

void MicroSwitch::Init()
{
    pinMode(m_buttonPin, INPUT_PULLUP);
}

ActionType MicroSwitch::PollForChange()
{
    ActionType result = ActionType::None;
    uint8_t actionState = digitalRead(m_buttonPin);
    if (actionState != m_lastButtonState)
    {
        if (actionState == LOW) {
            // switch is closed after being open
            result = ActionType::Closed;
        } else {
            // swich is open after being closed
            result = ActionType::Open;
        }
        m_lastButtonState = actionState;
    }
    return result;
}

ActionType MicroSwitch::IsOpen()
{
    return digitalRead(m_buttonPin) == HIGH;
}