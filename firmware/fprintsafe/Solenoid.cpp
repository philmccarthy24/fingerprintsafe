#include <Arduino.h>
#include "Solenoid.h"

Solenoid::Solenoid(uint8_t controlPin, long activationDurationMS) : 
    m_controlPin(controlPin),
    m_activationDurationMS(activationDurationMS),
    m_lastActivationTimeStamp(0)
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
    
}

void Solenoid::Poll()
{
    
}