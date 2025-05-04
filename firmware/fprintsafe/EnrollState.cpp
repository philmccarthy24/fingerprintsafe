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
  m_pSolenoid(pSolenoid)
{
}

EnrollState::~EnrollState()
{

}

void EnrollState::OnStateChanged()
{

}

void EnrollState::Poll()
{

}