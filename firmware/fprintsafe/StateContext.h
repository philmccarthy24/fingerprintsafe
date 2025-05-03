#ifndef HDR_STATECONTEXT_H
#define HDR_STATECONTEXT_H

#include "IStateContext.h"
#include "UnlockedState.h"
#include "LockedState.h"


// forward decs
class ILock;
class IStatusLed;

class StateContext : public IDeviceState, IStateContext
{
public:
  StateContext(ILock* pLock, IStatusLed* pStatusLed);
  virtual ~StateContext();

  void Initialise();

  virtual void OnStateChanged();
  virtual void Poll();

  virtual void UpdateState(EDeviceState newState);

private:
  UnlockedState m_unlockedState;
  LockedState m_lockedState;
  ChangePinState m_changePinState;

  ILock* m_pLock;
  ILockManagerState* m_pCurrentState;
};

#endif