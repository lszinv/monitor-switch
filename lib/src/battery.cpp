#include <windows.h>
#include <iostream>

bool IsLaptop() {
  SYSTEM_POWER_STATUS powerStatus;
  if (GetSystemPowerStatus(&powerStatus)) {
    return (powerStatus.BatteryFlag != 128); // 128 means no battery
  }
  return false;
}
