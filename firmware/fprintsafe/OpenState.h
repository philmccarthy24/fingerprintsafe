#ifndef HDR_OPENSTATE_H
#define HDR_OPENSTATE_H

#include "IStateContext.h"

// forward decs
class WvFingerprint;
class MicroSwitch;
class MomentarySwitch;
class PiezoBuzzer;
class Solenoid;

class OpenState : public IDeviceState
{
public:
  OpenState(IStateContext* pContext, WvFingerprint* pFingerprintSensor, MicroSwitch* pOpeningSwitch, MomentarySwitch* pControlButton, PiezoBuzzer* pBuzzer, Solenoid* pSolenoid);
  virtual ~OpenState();

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