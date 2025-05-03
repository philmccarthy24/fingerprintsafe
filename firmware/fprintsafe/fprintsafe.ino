#include "wvfpe.h"
#include "MomentarySwitch.h"
#include "MicroSwitch.h"
#include "Solenoid.h"
#include "PiezoBuzzer.h"

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
//State ////// need more work on defining states etc ............................

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
}

void loop() {

  ActionType as = ms.PollForAction();

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
  else 
  {

    int userId = fpsensor.checkForFingerprint();

    // userId = 0: no finger on device
    // userId = 0xFFFF: finger on device but no match  
    if (userId == 0xFFFF) {
        Serial.println("Finger found but unknown!");

    } else if (userId > 0) {
        Serial.print("user match. userId: ");
        Serial.println(userId);
        
    }
    
  }

}

