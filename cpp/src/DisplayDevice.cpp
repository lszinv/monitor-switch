#include "DisplayDevice.h"
#include "spdlog/spdlog.h"
#include <codecvt>
#include <iostream>
#include <locale>
#include <string>

using namespace MonitorSwitch;

std::string DisplayDevice::wstringToString(const std::wstring &wstr) {
  std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
  return converter.to_bytes(wstr);
}

DisplayDevice::DisplayDevice(HMONITOR monitor, std::wstring device_name) {
  this->monitor_handle = monitor;
  this->GetMonitorDetails(device_name.c_str());
  return;
}

void DisplayDevice::SetInputs(std::map<int, std::string> &&inputs) {
  this->inputs = std::move(inputs);
}

void DisplayDevice::SetCurrentInput(std::pair<int, std::string> current_input) {
  this->current_input = current_input;
}

void DisplayDevice::GetMonitorDetails(LPCWSTR device_name) {
  DISPLAY_DEVICEW dd = {sizeof(dd)};
  DWORD device_index = 0;
  bool found_active_device = false;
  while (EnumDisplayDevicesW(device_name, device_index, &dd,
                             EDD_GET_DEVICE_INTERFACE_NAME)) {
    this->device_string = wstringToString(dd.DeviceString);
    this->device_id = wstringToString(dd.DeviceID);
    this->device_key = wstringToString(dd.DeviceKey);
    device_index++;
    // If there are multiple displays to this physical device,
    // keep cycling until the active one is found
    if (dd.StateFlags & DISPLAY_DEVICE_ACTIVE) {
      this->device_id = wstringToString(dd.DeviceID);
      found_active_device = true;
      break;
    }
  }
  if (!found_active_device) {
    spdlog::error("No active device founds for " + wstringToString(device_name));
  }
}

std::ostream &
MonitorSwitch::operator<<(std::ostream &os,
                          const MonitorSwitch::DisplayDevice &dd) {
  os << "Display Device: " << dd.device_string << std::endl
     << "\tDevice ID: " << dd.device_id << std::endl
     << "\tDevice Key: " << dd.device_key << std::endl;
  return os;
}
