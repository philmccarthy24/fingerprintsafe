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

bool MicroSwitch::IsOpen()
{
    return digitalRead(m_buttonPin) == HIGH;
}