#include "wvfpe.h"
#include "MomentarySwitch.h"
#include "MicroSwitch.h"
#include "Solenoid.h"
#include "PiezoBuzzer.h"
#include "StateContext.h"

const int RST_PIN = D3; // ENABLE
const int WAKE_PIN = D2; // IRQ
const int PIEZO_BUZZER_PIN = D1;
const int ACTION_BUTTON = D8;
const int SOLENOID_MOSFET_PIN = D9;
const int OPEN_SWITCH = D10;

// create instance
WvFingerprint fpsensor(Serial1, RST_PIN, WAKE_PIN);
MomentarySwitch controlButton(ACTION_BUTTON, 3000);
MicroSwitch openingSwitch(OPEN_SWITCH);
PiezoBuzzer piezo(PIEZO_BUZZER_PIN);
Solenoid solenoid(SOLENOID_MOSFET_PIN, 5000);

StateContext stateMachine(&fpsensor, &controlButton, &openingSwitch, &piezo, &solenoid);

// Serial is on the USB port.
// Serial1 is the Xiao's pin 6(TX) and 7(RX).
// RX on sensor goes to TX (D6) on MC
// TX on sensor goes to RX (D7) on MC

void setup() {
  Serial.begin(115200);

  fpsensor.init();
  controlButton.Init();
  openingSwitch.Init();
  piezo.Init(); 
  solenoid.Init();
  stateMachine.Initialise();
}

void loop() {

  
  else 
  {

    
    
  }

}

