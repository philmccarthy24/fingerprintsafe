#include "OpenState.h"
#include "wvfpe.h"
#include "MicroSwitch.h"
#include "MomentarySwitch.h"
#include "PiezoBuzzer.h"
#include "Solenoid.h"

OpenState::OpenState(IStateContext* pContext, WvFingerprint* pFingerprintSensor, MicroSwitch* pOpeningSwitch, MomentarySwitch* pControlButton, PiezoBuzzer* pBuzzer, Solenoid* pSolenoid) :
  m_pContext(pContext),
  m_pFingerprintSensor(pFingerprintSensor), 
  m_pOpeningSwitch(pOpeningSwitch),
  m_pControlButton(pControlButton),
  m_pBuzzer(pBuzzer),
  m_pSolenoid(pSolenoid)
{
}

OpenState::~OpenState()
{

}

void OpenState::OnStateChanged()
{
  Serial.println("OpenState entry");
}

void OpenState::Poll()
{
  ActionType as = m_pControlButton->PollForAction();

  if (as == ActionType::Pushed) {
    Serial.println("Enrolling new user. Place your finger on the sensor and press the button again.");
    m_pContext->UpdateState(EDeviceState::EnrollNewUser);
  }
  else if (as == ActionType::HeldDown) {
    m_pFingerprintSensor->clearAllUser();
    m_pBuzzer->PlayReset();
    Serial.println("All users removed from fingerprint scanner.");
  }
  
  if (!m_pOpeningSwitch->IsOpen())
  {
    m_pContext->UpdateState(EDeviceState::Locked);
  }
}