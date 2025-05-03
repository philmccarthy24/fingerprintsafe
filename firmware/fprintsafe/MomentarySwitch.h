#ifndef momentaryswitch_h
#define momentaryswitch_h

#include <Arduino.h>

typedef enum _ActionType {
    None,
    Pushed,
    HeldDown
} ActionType;

// Represents an active low momentary switch which triggers when either the switch
// comes up after being depressed (Pushed), or after being held down for a long enough
// time (HeldDown)
class MomentarySwitch {
    public:
        MomentarySwitch(uint8_t buttonPin, long longPressMS);
        virtual ~MomentarySwitch();
        void Init();
        ActionType PollForAction();

    private:
        uint8_t m_buttonPin;
        uint8_t m_lastButtonState;
        long m_longPressMS;
        long m_activeTimestamp;
        bool m_longPressTriggered;
};

#endif