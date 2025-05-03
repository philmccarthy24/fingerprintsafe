#include <Arduino.h>
#include "button.h"

MomentarySwitch::MomentarySwitch(uint8_t buttonPin, long longPressMS) :
    m_buttonPin(buttonPin),
    m_longPressMS(longPressMS),
    m_lastButtonState(HIGH),
    m_activeTimestamp(0)
{
}

MomentarySwitch::~MomentarySwitch()
{
}

void MomentarySwitch::Init()
{
    pinMode(m_buttonPin, INPUT_PULLUP);
}

ActionType MomentarySwitch::PollForAction()
{
    ActionType result = ActionType::None;
    uint8_t actionState = digitalRead(m_buttonPin);
    if (actionState != m_lastButtonState)
    {
        if (actionState == LOW) {
            // button is down
            m_activeTimestamp = millis();
        } else {
            // button has just come up from being down
            if (millis() - m_activeTimestamp >= m_longPressMS) {
                result = ActionType::LongPress;
            } else {
                result = ActionType::ShortPress;
            }
        }
        m_lastButtonState = actionState;
    }
    return result;
}