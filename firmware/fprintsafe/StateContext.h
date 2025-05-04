#ifndef HDR_STATECONTEXT_H
#define HDR_STATECONTEXT_H

#include "IStateContext.h"
#include "OpenState.h"
#include "LockedState.h"
#include "EnrollState.h"

// forward decs
class WvFingerprint;
class MomentarySwitch;
class MicroSwitch;
class PiezoBuzzer;
class Solenoid;

// it would be nice to house the low level device abstractions in this class so it's more self
// contained and ownership isn't bled into the main sketch... but might not be possible
class StateContext : public IDeviceState, IStateContext
{
public:
  StateContext(WvFingerprint* pFingerprintSensor, MomentarySwitch* pControlButton, MicroSwitch* pOpeningSwitch, PiezoBuzzer* pBuzzer, Solenoid* pSolenoid);
  virtual ~StateContext();

  void Initialise();

  virtual void OnStateChanged();
  virtual void Poll();

  virtual void UpdateState(EDeviceState newState);

private:
  OpenState m_openState;
  LockedState m_lockedState;
  EnrollState m_enrollState;

  IDeviceState* m_pCurrentState;

  WvFingerprint* m_pFingerprintSensor;
  MomentarySwitch* m_pControlButton;
  MicroSwitch* m_pOpeningSwitch;
  PiezoBuzzer* m_pBuzzer;
  Solenoid* m_pSolenoid;
};

#endif