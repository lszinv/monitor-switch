#pragma once
#include "DisplayDevice.h"
#include "DisplayDeviceId.h"
#include "mccs/base/InputSource.h"
#include <vector>
#include <windef.h>
#include <windows.h>
// TODO: This needs to be a singleton because the win commands have to be static.
// meaning the static callback won't be able to interact with the object members
namespace MonitorSwitch {
  class DisplayDeviceManager {
  public:
    static std::vector<DisplayDevice> display_devices;

    static std::shared_ptr<DisplayDeviceManager> GetDisplayDeviceManager();

    DisplayDeviceManager();

    /**
       Scans existing monitors and matches with existing devices
       where it can. Otherwise, adds new entries, and removes stale entries.
       Stale entry removal should be soft, and kept in case it is connected again.
    */
    void Scan();

    void ResetHandles();

    DisplayDevice *GetDisplayDevice(DeviceUId device_id);

    const std::map<DeviceUId, DisplayDevice>& GetDevices() const;

    void ChangeDisplayInput(std::string device_id, InputType input);

  private:

    static std::shared_ptr<DisplayDeviceManager> cached;

    static BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor,
                                         LPRECT lprcMonitor, LPARAM dwData);

    std::map<DeviceUId, DisplayDevice> devices;
    void UpdateDevice(DisplayDevice&& device);
  };
} // namespace MonitorSwitch
