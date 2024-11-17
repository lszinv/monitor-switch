#pragma once
#include <map>
#include <string>
#include <windows.h>
#include <iostream>

namespace MonitorSwitch {
  class DisplayDevice {
  public:
    DisplayDevice(HMONITOR monitor, std::wstring device_name);

    HMONITOR monitor_handle;
    std::wstring device_name;
    std::wstring name;
    std::wstring device_string;
    std::wstring device_id;
    std::wstring device_key;

    void SetInputs(std::map<int, std::wstring> &&);
    void SetCurrentInput(std::pair<int, std::wstring>);

  private:
    std::map<int /* Code */, std::wstring /* Input Label  */> inputs;
    std::pair<int /* Code */, std::wstring /* Input Label */> current_input;
    void GetMonitorDetails();
  };

  std::wostream& operator<<(std::wostream& os, const MonitorSwitch::DisplayDevice& dd);
} // namespace MonitorSwitch
