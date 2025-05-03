#ifndef HDR_ISOLENOID_H
#define HDR_ISOLENOID_H

class Solenoid
{
public:
  Solenoid(uint8_t buzzerPin);
  virtual ~Solenoid();

  void Activate();
  void Deactivate();
};

#endif