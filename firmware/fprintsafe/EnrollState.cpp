#include "EnrollState.h"
#include "wvfpe.h"
#include "MicroSwitch.h"
#include "MomentarySwitch.h"
#include "PiezoBuzzer.h"
#include "Solenoid.h"

EnrollState::EnrollState(IStateContext* pContext, WvFingerprint* pFingerprintSensor, MicroSwitch* pOpeningSwitch, MomentarySwitch* pControlButton, PiezoBuzzer* pBuzzer, Solenoid* pSolenoid) :
  m_pContext(pContext),
  m_pFingerprintSensor(pFingerprintSensor), 
  m_pOpeningSwitch(pOpeningSwitch),
  m_pControlButton(pControlButton),
  m_pBuzzer(pBuzzer),
  m_pSolenoid(pSolenoid),
  m_enrollStartTimestamp(0)
{
}

EnrollState::~EnrollState()
{

}

void EnrollState::OnStateChanged()
{
  Serial.println("EnrollState entry");
  m_pBuzzer->PlayReady();
  m_enrollStartTimestamp = millis();
}

void EnrollState::Poll()
{
  bool transitionRequired = false;
  if ((millis() - m_enrollStartTimestamp) >= ENROLL_STATE_TIMEOUT_MS)
  {
    // timed out
    m_pBuzzer->PlayError();
    transitionRequired = true;
  } 
  else 
  {

    ActionType as = m_pControlButton->PollForAction();

    if (as == ActionType::Pushed) {
      int16_t numUsers = m_pFingerprintSensor->getUserCount();
      Serial.print("Num users = ");
      Serial.println(numUsers);

      delay(1000);
      
      bool addResult = false;
      for (int i = 1; i < 4; i++) {
        addResult = m_pFingerprintSensor->addUser(numUsers+1, i);
        if (!addResult) {
          Serial.print("Add step ");
          Serial.print(i);
          Serial.println(" failed");
          
          break;
        }
      }
      if (addResult) {
        m_pBuzzer->PlaySuccess();
        
        Serial.print("User id ");
        Serial.print(numUsers+1);
        Serial.println(" added.");
      } 
      else 
      {
        m_pBuzzer->PlayError();
      }
      transitionRequired = true;
    }
  }

  if (transitionRequired == true) {
    if (m_pOpeningSwitch->IsOpen()) {
      m_pContext->UpdateState(EDeviceState::Unlocked);
    } else {
      m_pContext->UpdateState(EDeviceState::Locked);
    }
  }
}