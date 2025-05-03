#ifndef momentaryswitch_h
#define momentaryswitch_h

#include <Arduino.h>

typedef enum _ActionType {
    None,
    ShortPress,
    LongPress
} ActionType;

// Represents an active low momentary switch
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
};

#endif