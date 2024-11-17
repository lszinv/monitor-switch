#include "DisplayDeviceManager.h"
#include "DisplayDevice.h"
#include <iostream>

using namespace MonitorSwitch;

std::vector<DisplayDevice> DisplayDeviceManager::display_devices = {};

DisplayDeviceManager::DisplayDeviceManager() { return; };

void DisplayDeviceManager::Scan() {
  EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, 0);
  return;
}

BOOL CALLBACK DisplayDeviceManager::MonitorEnumProc(HMONITOR hMonitor,
                                                    HDC hdcMonitor,
                                                    LPRECT lprcMonitor,
                                                    LPARAM dwData) {
  MONITORINFOEXW mi;
  mi.cbSize = sizeof(MONITORINFOEXW);
  if (GetMonitorInfoW(hMonitor, &mi)) {
    DisplayDevice dd(hMonitor, mi.szDevice);
    display_devices.push_back(dd);
  }
  return TRUE;
}

DisplayDevice *DisplayDeviceManager::GetDisplayDevice(std::wstring device_id) {
  return &(this->display_devices[0]);
}
