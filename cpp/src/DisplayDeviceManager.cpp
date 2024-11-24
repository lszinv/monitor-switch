#include "DisplayDeviceManager.h"
#include "DisplayDevice.h"
#include <iostream>
#include <codecvt>
#include <locale>
#include <string>

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

void DisplayDeviceManager::RefreshHandles() {
  // std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
  // std::wstring wstr = converter.from_bytes(device_id);


  return;
}

DisplayDevice *DisplayDeviceManager::GetDisplayDevice(std::wstring device_id) {
  return &(this->display_devices[0]);
}
