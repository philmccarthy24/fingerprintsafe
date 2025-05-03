#include "wvfpe.h"
#include "button.h"
#include "piezobuzzer.h"

const int RST_PIN = D3; // ENABLE
const int WAKE_PIN = D2; // IRQ
const int PIEZO_BUZZER_PIN = D1;
const int ACTION_BUTTON = D8;

// create instance
WvFingerprint fpsensor(Serial1, RST_PIN, WAKE_PIN);
MomentarySwitch ms(ACTION_BUTTON, 3000);
PiezoBuzzer piezo(PIEZO_BUZZER_PIN);

// Serial is on the USB port.
// Serial1 is the Xiao's pin 6(TX) and 7(RX).
// RX on sensor goes to TX (D6) on MC
// TX on sensor goes to RX (D7) on MC

void setup() {
  Serial.begin(115200);

  fpsensor.init();
  ms.Init();
  piezo.Init();

  
}

// I got the enrollment to work by disabling sleep mode and adding digitalWrite(_rstPin, 1); in init() to always
// have sensor awake.
// possibly the sleep/wake state changing is borking enrollment

// actually code that set sleep mode by writing to RST_PIN was causing weird behaviour full stop.
// not using sleep mode at all got fingerprint recognition working (just polling WVFP_CMD_MATCH).
// This said, the WAKE pin goes high when RST low (in sleep) and finger on sensor. How can we make this more reliable?

// AHHH it was insufficient delay from waking up from sleep mode.
// made it 200ms instead of 50 and it seems to be reliably recognising my index finger now.

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

