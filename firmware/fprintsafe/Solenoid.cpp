#include <Arduino.h>
#include "Solenoid.h"

Solenoid::Solenoid(uint8_t controlPin, long activationDurationMS) : 
    m_controlPin(controlPin),
    m_activationDurationMS(activationDurationMS),
    m_lastActivationTimeStamp(0),
    m_bActivated(false)
{
}

Solenoid::~Solenoid()
{
}

void Solenoid::Init()
{
    pinMode(m_controlPin, OUTPUT);
}

void Solenoid::Activate()
{
    digitalWrite(m_controlPin, HIGH);
    m_lastActivationTimeStamp = millis();
    m_bActivated = true;
}

bool Solenoid::IsActivated()
{
    return m_bActivated;
}

void Solenoid::Poll()
{
    if (m_bActivated && (millis() - m_lastActivationTimeStamp) >= m_activationDurationMS)
    {
        digitalWrite(m_controlPin, LOW);
        m_bActivated = false;
    }
}