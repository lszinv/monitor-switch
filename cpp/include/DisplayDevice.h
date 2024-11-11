#pragma once
#include <string>
#include <windows.h>
#include <map>

namespace MonitorSwitch {
class DisplayDevice {
public:
  DisplayDevice();

  void setPhysicalMonitor(HANDLE);
  void setDisplayMonitor(HMONITOR);
  void setName(std::wstring);
  void setDeviceString(std::wstring);
  void setDeviceId(std::wstring);
  void setInputs(std::map<int, std::wstring>&&);
  void setCurrentInput(std::pair<int, std::wstring>);

private:
  HANDLE physical_monitor;
  HMONITOR display_monitor;
  std::wstring name;
  std::wstring device_string;
  std::wstring device_id;
  std::map<int /* Code */, std::wstring /* Input Label  */> inputs;
  std::pair<int /* Code */, std::wstring /* Input Label */> current_input;
};
}
