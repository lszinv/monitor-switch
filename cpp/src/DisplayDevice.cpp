#include "DisplayDevice.h"
#include <iostream>

using namespace MonitorSwitch;

DisplayDevice::DisplayDevice(HMONITOR monitor, std::wstring device_name)
    : monitor_handle(monitor), device_name(device_name) {
  this->GetMonitorDetails();
  return;
}

void DisplayDevice::SetInputs(std::map<int, std::wstring> &&inputs) {
  this->inputs = std::move(inputs);
}

void DisplayDevice::SetCurrentInput(
    std::pair<int, std::wstring> current_input) {
  this->current_input = current_input;
}

void DisplayDevice::GetMonitorDetails() {
  DISPLAY_DEVICEW dd = {sizeof(dd)};
  DWORD device_index = 0;
  bool found_active_device = false;
  while (EnumDisplayDevicesW(this->device_name.c_str(), device_index, &dd,
                             EDD_GET_DEVICE_INTERFACE_NAME)) {
    this->device_string = dd.DeviceString;
    this->device_id = dd.DeviceID;
    this->device_key = dd.DeviceKey;
    device_index++;
    // If there are multiple displays to this physical device,
    // keep cycling until the active one is found
    if (dd.StateFlags & DISPLAY_DEVICE_ACTIVE) {
      this->device_id = dd.DeviceID;
      found_active_device = true;
      break;
    }
  }
  if (!found_active_device) {
    // Log problem
  }
}

std::wostream &
MonitorSwitch::operator<<(std::wostream &os,
                          const MonitorSwitch::DisplayDevice &dd) {
  os << "Display Device: " << dd.device_name << ", "
     << "Device String: " << dd.device_string << ", "
     << "Device ID: " << dd.device_id << ", " << "Device Key: " << dd.device_key
     << std::endl;
  return os;
}
