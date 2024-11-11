#pragma once
#include "DisplayDevice.h"
#include <vector>

namespace MonitorSwitch {
  class DisplayDeviceManager {
  public:
    DisplayDeviceManager();

    /**
       Scans existing monitors and matches with existing devices
       where it can. Otherwise, adds new entries, and removes stale entries.
       Stale entry removal should be soft, and kept in case it is connected again.
    */
    void Scan();

    DisplayDevice* GetDisplayDevice(std::wstring device_id);

  private:
    // TODO: Maybe make this a map to be sorted and searchable?
    std::vector<DisplayDevice> display_devices;

    void updateDevices(DisplayDevice);

  };
}
