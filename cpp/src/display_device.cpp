#include <windows.h>
#include <iostream>

void EnumerateDisplayDevices() {
  DISPLAY_DEVICE dd;
  dd.cb = sizeof(DISPLAY_DEVICE);

  for (int i = 0; EnumDisplayDevices(NULL, i, &dd, 0); i++) {
    if (dd.StateFlags & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP) {
      std::wcout << L"Device Name: " << dd.DeviceName << std::endl;
      std::wcout << L"Device String: " << dd.DeviceString << std::endl;
      std::wcout << L"Device ID: " << dd.DeviceID << std::endl;
      std::wcout << L"Device Key: " << dd.DeviceKey << std::endl;

      if (dd.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE) {
        std::wcout << L"This is the primary display device" << std::endl;
      }

      std::wcout << std::endl;
    }
  }
}
