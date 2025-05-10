#ifndef microswitch_h
#define microswitch_h

#include <Arduino.h>

// Represents a simple active-low micro switch / reed relay etc which changes state based on being opened or closed
class MicroSwitch {
    public:
        MicroSwitch(uint8_t buttonPin);
        virtual ~MicroSwitch();
        void Init();
        bool IsOpen();

    private:
        uint8_t m_buttonPin;
        uint8_t m_lastButtonState;
};

#endif