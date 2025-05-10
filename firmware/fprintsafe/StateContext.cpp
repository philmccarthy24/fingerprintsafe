#include "StateContext.h"
#include "wvfpe.h"
#include "MicroSwitch.h"
#include "MomentarySwitch.h"
#include "PiezoBuzzer.h"
#include "Solenoid.h"

StateContext::StateContext(WvFingerprint* pFingerprintSensor, MomentarySwitch* pControlButton, MicroSwitch* pOpeningSwitch, PiezoBuzzer* pBuzzer, Solenoid* pSolenoid) :
  m_openState(this, pFingerprintSensor, pOpeningSwitch, pControlButton, pBuzzer, pSolenoid),
  m_lockedState(this, pFingerprintSensor, pOpeningSwitch, pControlButton, pBuzzer, pSolenoid),
  m_enrollState(this, pFingerprintSensor, pOpeningSwitch, pControlButton, pBuzzer, pSolenoid),
  m_pFingerprintSensor(pFingerprintSensor),
  m_pControlButton(pControlButton),
  m_pOpeningSwitch(pOpeningSwitch),
  m_pBuzzer(pBuzzer),
  m_pSolenoid(pSolenoid)
{
}

StateContext::~StateContext()
{
}

void StateContext::Initialise()
{
  if (m_pOpeningSwitch->IsOpen())
  {
    m_pCurrentState = &m_openState;
  } else {
    m_pCurrentState = &m_lockedState;
  }
  m_pCurrentState->OnStateChanged();
}

void StateContext::OnStateChanged()
{
  // should never be called on main context
}

void StateContext::UpdateState(EDeviceState newState)
{
  switch (newState) {
    case Unlocked :
      m_pCurrentState = &m_openState;
      break;
    case Locked :
      m_pCurrentState = &m_lockedState;
      break;
    case EnrollNewUser :
      m_pCurrentState = &m_enrollState;
      break;
  }
  // call OnStateChanged to allow the state to carry out any book keeping / initialisation it needs
  m_pCurrentState->OnStateChanged();
}

void StateContext::Poll()
{
  m_pSolenoid->Poll();
  m_pCurrentState->Poll();
}