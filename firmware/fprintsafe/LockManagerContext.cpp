#include "LockManagerContext.h"
#include "IStatusLed.h"
#include "ILock.h"
#include <EEPROM.h>

LockManagerContext::LockManagerContext(ILock* pLock, IStatusLed* pStatusLed) :
m_unlockedState(this, pLock, pStatusLed),
m_lockedState(this, pLock, pStatusLed),
m_changePinState(this, pStatusLed),
m_pLock(pLock)
{
}

LockManagerContext::~LockManagerContext()
{
}

void LockManagerContext::Initialise()
{
  if (m_pLock->IsLocked())
  {
    m_pCurrentState = &m_lockedState;
  } else {
    m_pCurrentState = &m_unlockedState;
  }
  m_pCurrentState->OnStateChanged();

  // retrieve and validate pin
  EEPROM.get(PIN_EEPROM_ADDRESS, m_pin);
  bool isValid = true;
  for (int i = 0; i < 4; i++)
  {
    if (m_pin[i] < 48 || m_pin[i] > 57)
    {
      isValid = false;
      break;
    }
  }
  if (!isValid) {
    Serial.println("Initialising unset or corrupt pin to 0000");
    // uninitialised - set to "0000"
    SetPin("0000");
  }
}

void LockManagerContext::OnStateChanged()
{
  // should never be called on main context
}

void LockManagerContext::HandleKeypress(char k)
{
  m_pCurrentState->HandleKeypress(k);
}

void LockManagerContext::CheckTimeout()
{
  m_pCurrentState->CheckTimeout();
}

void LockManagerContext::UpdateState(ELockState newState)
{
  switch (newState) {
    case Unlocked :
      m_pCurrentState = &m_unlockedState;
      break;
    case Locked :
      m_pCurrentState = &m_lockedState;
      break;
    case ChangePin :
      m_pCurrentState = &m_changePinState;
      break;
  }
  // call OnStateChanged to allow the state to carry out any book keeping / initialisation it needs
  m_pCurrentState->OnStateChanged();
}

