#include "LockedState.h"
#include "wvfpe.h"
#include "MicroSwitch.h"
#include "MomentarySwitch.h"
#include "PiezoBuzzer.h"
#include "Solenoid.h"

LockedState::LockedState(IStateContext* pContext, WvFingerprint* pFingerprintSensor, MicroSwitch* pOpeningSwitch, MomentarySwitch* pControlButton, PiezoBuzzer* pBuzzer, Solenoid* pSolenoid) :
  m_pContext(pContext),
  m_pFingerprintSensor(pFingerprintSensor), 
  m_pOpeningSwitch(pOpeningSwitch),
  m_pControlButton(pControlButton),
  m_pBuzzer(pBuzzer),
  m_pSolenoid(pSolenoid)
{
}

LockedState::~LockedState()
{
}

void LockedState::OnStateChanged()
{
  Serial.println("LockedState entry");
}

void LockedState::Poll()
{
  int userId = m_pFingerprintSensor->checkForFingerprint();
  
  // userId = 0xFFFF: finger on device but no match  
  if (userId == 0xFFFF) {
      Serial.println("Finger found but unknown!");    
  } else if (userId > 0) {
      Serial.print("user match. userId: ");
      Serial.println(userId);

      // unlock
      m_pSolenoid->Activate(); // this will physically allow the opening switch to be triggered from manual door/drawer pull-open
  } // else userId = 0: no finger on device. do nothing

  // NOTE solenoid is polled in context
  
  if (m_pOpeningSwitch->IsOpen() && m_pSolenoid->IsActivated()) {
    m_pContext->UpdateState(EDeviceState::Unlocked);
  }
}