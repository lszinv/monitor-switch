#pragma once
#include <map>
#include <string>
#include <windows.h>
#include <iostream>

namespace MonitorSwitch {
  class DisplayDevice {
  public:
    DisplayDevice(HMONITOR monitor, std::wstring device_name);
    DisplayDevice(std::string device_id,
                  std::string device_key,
                  std::string device_string);

    HMONITOR monitor_handle;
    std::string device_name;
    std::string device_string;
    std::string device_id;
    std::string device_key;

    void SetInputs(std::map<int, std::string> &&);
    void SetCurrentInput(std::pair<int, std::string>);

  private:
    std::map<int /* Code */, std::string /* Input Label  */> inputs;
    std::pair<int /* Code */, std::string /* Input Label */> current_input;
    void GetMonitorDetails(LPCWSTR device_name);
    std::string wstringToString(const std::wstring& wstr);
  };

  std::ostream& operator<<(std::ostream& os, const MonitorSwitch::DisplayDevice& dd);
} // namespace MonitorSwitch
