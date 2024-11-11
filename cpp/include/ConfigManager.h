#pragma once
#include "DisplayDevice.h"
#include "DisplayDeviceManager.h"
#include <vector>


namespace MonitorSwitch {
  class ConfigManager {
  public:
    ConfigManager();
    void SaveConfig(DisplayDeviceManager, std::string);
    DisplayDeviceManager LoadConfig(std::string);
  private:
  };
}
