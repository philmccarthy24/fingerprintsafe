#ifndef HDR_LOCKMANAGER_H
#define HDR_LOCKMANAGER_H

#include "ILockManager.h"
#include "UnlockedState.h"
#include "LockedState.h"
#include "ChangePinState.h"


// forward decs
class ILock;
class IStatusLed;

class LockManagerContext : public ILockManagerState, ILockManagerContext
{
public:
  LockManagerContext(ILock* pLock, IStatusLed* pStatusLed);
  virtual ~LockManagerContext();

  void Initialise();

  virtual void OnStateChanged();
  virtual void HandleKeypress(char k);
  virtual void CheckTimeout();

  virtual void UpdateState(ELockState newState);
  virtual void SetPin(const char* newPin);
  virtual const char* GetPin();

private:
  UnlockedState m_unlockedState;
  LockedState m_lockedState;
  ChangePinState m_changePinState;

  ILock* m_pLock;
  ILockManagerState* m_pCurrentState;
};

#endif