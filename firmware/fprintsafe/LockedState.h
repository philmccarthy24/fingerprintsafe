#ifndef HDR_LOCKEDSTATE_H
#define HDR_LOCKEDSTATE_H

#include "ILockManager.h"

// forward decs
class ILock;
class IStatusLed;
class ILockManagerContext;

class LockedState : public ILockManagerState
{
public:
  LockedState(ILockManagerContext* pContext, ILock* pLock, IStatusLed* pStatusLed);
  virtual ~LockedState();

  virtual void OnStateChanged();
  virtual void HandleKeypress(char k);
  virtual void CheckTimeout();

private:
  ILockManagerContext* m_pContext;
  ILock* m_pLock;
  IStatusLed* m_pStatusLed;

  String m_keyBuffer;
  unsigned long m_lastKeyTime;
};

#endif