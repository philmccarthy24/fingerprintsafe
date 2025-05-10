#ifndef HDR_ENROLLSTATE_H
#define HDR_ENROLLSTATE_H

#include "IStateContext.h"

// forward decs
class WvFingerprint;
class MicroSwitch;
class MomentarySwitch;
class PiezoBuzzer;
class Solenoid;

class EnrollState : public IDeviceState
{
public:
  EnrollState(IStateContext* pContext, WvFingerprint* pFingerprintSensor, MicroSwitch* pOpeningSwitch, MomentarySwitch* pControlButton, PiezoBuzzer* pBuzzer, Solenoid* pSolenoid);
  virtual ~EnrollState();

  virtual void OnStateChanged();
  virtual void Poll();

private:
  IStateContext* m_pContext;
  WvFingerprint* m_pFingerprintSensor;
  MicroSwitch* m_pOpeningSwitch;
  MomentarySwitch* m_pControlButton;
  PiezoBuzzer* m_pBuzzer;
  Solenoid* m_pSolenoid;
  long m_enrollStartTimestamp;

  static const long ENROLL_STATE_TIMEOUT_MS = 10000;
};

#endif