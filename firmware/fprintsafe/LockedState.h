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
  LockedState(IStateContext* pContext, WvFingerprint* pFingerprintSensor, MicroSwitch* pOpeningSwitch, MomentarySwitch* pControlButton, PiezoBuzzer* pBuzzer, Solenoid* pSolenoid);
  virtual ~LockedState();

  virtual void OnStateChanged();
  virtual void Poll();

private:
  IStateContext* m_pContext;
  WvFingerprint* m_pFingerprintSensor;
  MicroSwitch* m_pOpeningSwitch;
  MomentarySwitch* m_pControlButton;
  PiezoBuzzer* m_pBuzzer;
  Solenoid* m_pSolenoid;
};

#endif