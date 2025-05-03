#include "LockedState.h"
#include "IStatusLed.h"
#include "ILock.h"

LockedState::LockedState(ILockManagerContext* pContext, ILock* pLock, IStatusLed* pStatusLed) :
m_pContext(pContext),
m_pLock(pLock),
m_pStatusLed(pStatusLed)
{
  m_keyBuffer.reserve(9);
  m_keyBuffer = "";
  m_lastKeyTime = millis();
}

LockedState::~LockedState()
{
}

void LockedState::OnStateChanged()
{
  m_keyBuffer = "";
  m_lastKeyTime = millis();
  m_pStatusLed->SetColour(RED);
}

void LockedState::HandleKeypress(char k)
{
  static const String emergencyOverride = "18022016";

  // stop keypress timeout
  m_lastKeyTime = millis();

  m_keyBuffer += String(k);
  m_pStatusLed->Pulse();

  if (m_keyBuffer.equals(m_pContext->GetPin()) || m_keyBuffer.equals(emergencyOverride))
  {
    // user wants to unlock. Move servo to locking position
    m_pLock->Unlock();
    m_pContext->UpdateState(ELockState::Unlocked);
  } 
  else if (m_keyBuffer.length() > 3 && !emergencyOverride.startsWith(m_keyBuffer))
  {
    // reached 4 digits without match - reset buffer
    m_keyBuffer = "";
    m_pStatusLed->SetColour(ORANGE);
    m_pStatusLed->SetBlink(SLOW_BLINK);
    m_pStatusLed->SetColour(RED);
  }
}

void LockedState::CheckTimeout()
{
  unsigned long currTime = millis();
  if (m_keyBuffer.length() > 0 && currTime > m_lastKeyTime + PIN_ENTRY_TIMEOUT)
  {
    // reset buffer
    m_keyBuffer = "";
    m_pStatusLed->SetColour(ORANGE);
    m_pStatusLed->SetBlink(SLOW_BLINK);
    m_pStatusLed->SetColour(RED);
  }
}