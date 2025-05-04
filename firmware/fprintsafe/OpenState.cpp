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

}

void OpenState::Poll()
{
  m_pSolenoid->Poll();

ActionType as = ms.PollForAction();

//////// TODO still going

  if (as == ActionType::ShortPress) {
    piezo.PlayPress();
    Serial.println("Enrolling new user. Place your finger on the sensor.");

    int16_t numUsers = fpsensor.getUserCount();
    Serial.print("Num users = ");
    Serial.println(numUsers);
    
    delay(3000);
    
    bool addResult = false;
    for (int i = 1; i < 4; i++) {
      addResult = fpsensor.addUser(numUsers+1, i);
      if (!addResult) {
        Serial.print("Add step ");
        Serial.print(i);
        Serial.println(" failed");
        piezo.PlayError();
        return;
      }
      Serial.print("Remove your finger then place it back on the sensor.");
    }
    
    piezo.PlaySuccess();
    Serial.print("User id ");
    Serial.print(numUsers+1);
    Serial.println(" added.");
    
  }
  else if (as == ActionType::LongPress) {
    fpsensor.clearAllUser();
    piezo.PlayReset();
    Serial.println("All users removed from fingerprint scanner.");
  }
}