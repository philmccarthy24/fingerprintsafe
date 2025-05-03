#ifndef HDR_LOCKEDSTATE_H
#define HDR_LOCKEDSTATE_H

#include "IStateContext.h"

// forward decs
class ILock;
class IStatusLed;
class ILockManagerContext;

class WvFingerprint;
class MicroSwitch;
class MomentarySwitch;
class PiezoBuzzer;
class Solenoid;

class LockedState : public IDeviceState
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