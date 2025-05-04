#ifndef HDR_SOLENOID_H
#define HDR_SOLENOID_H

class Solenoid
{
public:
  Solenoid(uint8_t controlPin, long activationDurationMS);
  virtual ~Solenoid();

  void Init();
  void Activate();
  void IsActivated();
  void Poll();

private:
  uint8_t m_controlPin;
  long m_activationDurationMS;
  long m_lastActivationTimeStamp;
  bool m_bActivated;
};

#endif