#ifndef HDR_ISTATEMANAGER_H
#define HDR_ISTATEMANAGER_H

#include <Arduino.h>

//#define PIN_ENTRY_TIMEOUT 2000

typedef enum _EDeviceState
{
  Locked,
  Unlocked,
  EnrollNewUser
} EDeviceState;

class IStateContext
{
public:
  virtual ~IStateContext() {}

  virtual void UpdateState(EDeviceState newState) = 0;
};

class IDeviceState
{
public:
  virtual ~IDeviceState() {}

  virtual void OnStateChanged() = 0;
  virtual void Poll() = 0;
};

#endif