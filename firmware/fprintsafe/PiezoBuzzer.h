#ifndef piezobuzzer_h
#define piezobuzzer_h

#include <Arduino.h>

// Represents a piezo buzzer for audio feedback
// https://www.szynalski.com/tone-generator/ for hz of different tones
class PiezoBuzzer {
    public:
        PiezoBuzzer(uint8_t buzzerPin);
        virtual ~PiezoBuzzer();
        void Init();
        void PlayReady();
        void PlaySuccess();
        void PlayError();
        void PlayReset();

    private:
        uint8_t m_buzzerPin;
};

#endif