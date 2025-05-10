#include <Arduino.h>
#include "piezobuzzer.h"

PiezoBuzzer::PiezoBuzzer(uint8_t buzzerPin) : 
    m_buzzerPin(buzzerPin)
{
}

PiezoBuzzer::~PiezoBuzzer()
{
}

void PiezoBuzzer::Init()
{
    pinMode(m_buzzerPin, OUTPUT);
}

void PiezoBuzzer::PlaySuccess()
{
    tone(m_buzzerPin, 247);  // B3
    delay(75);
    noTone(m_buzzerPin);
    delay(25);
    tone(m_buzzerPin, 494); // B4
    delay(75);
    noTone(m_buzzerPin);
}

void PiezoBuzzer::PlayReady()
{
    tone(m_buzzerPin, 28);  // A0
    delay(20);
    noTone(m_buzzerPin);
    delay(100);
    tone(m_buzzerPin, 28);  // A0
    delay(20);
    noTone(m_buzzerPin);
    delay(100);
    tone(m_buzzerPin, 28);  // A0
    delay(20);
    noTone(m_buzzerPin);
}

void PiezoBuzzer::PlayError()
{
    tone(m_buzzerPin, 370);  // F#4
    delay(100);
    noTone(m_buzzerPin);
    delay(200);
    tone(m_buzzerPin, 330); // E4
    delay(300);
    noTone(m_buzzerPin);
}

void PiezoBuzzer::PlayReset()
{
    tone(m_buzzerPin, 55);  // A1
    delay(800);
    noTone(m_buzzerPin);
}