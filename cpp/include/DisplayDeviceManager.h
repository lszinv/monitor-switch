#pragma once
#include "DisplayDevice.h"
#include <vector>
#include <windef.h>
#include <windows.h>
// TODO: This needs to be a singleton because the win commands have to be static.
// meaning the static callback won't be able to interact with the object members
namespace MonitorSwitch {
class DisplayDeviceManager {
public:
  static std::vector<DisplayDevice> display_devices;
  static std::map<
                  std::string /* Device ID */,
                  DisplayDevice> display_device_map;


  DisplayDeviceManager();

  /**
     Scans existing monitors and matches with existing devices
     where it can. Otherwise, adds new entries, and removes stale entries.
     Stale entry removal should be soft, and kept in case it is connected again.
  */
  void Scan();

  void RefreshHandles();

  DisplayDevice *GetDisplayDevice(std::wstring device_id);

private:
  // Win Call
  static BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor,
                                       LPRECT lprcMonitor, LPARAM dwData);

  void updateDevices(DisplayDevice);
};
} // namespace MonitorSwitch
