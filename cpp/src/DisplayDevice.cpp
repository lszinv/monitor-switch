#include "DisplayDevice.h"
#include "DisplayDeviceId.h"
#include "spdlog/spdlog.h"
#include <codecvt>
#include <iostream>
#include <locale>
#include <sstream>
#include <string>
#include <vector>
#include <lowlevelmonitorconfigurationapi.h>

// For some reason this function is missing from mingw32's
// highlevelmonitorconfigurationapi.h, so we're declaring it here.
extern "C" {
BOOL GetCapabilitiesStringLength(
    HANDLE hMonitor, LPDWORD pdwCapabilitiesStringLengthInCharacters);
}

using namespace MonitorSwitch;

std::string DisplayDevice::wstringToString(const std::wstring &wstr) {
  std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
  return converter.to_bytes(wstr);
}

DisplayDevice::DisplayDevice(HMONITOR monitor, std::wstring device_name) {
  this->handle = monitor;
  this->GetMonitorDetails(device_name.c_str());
  this->LoadInputs();
  return;
}

DisplayDevice::DisplayDevice(const DisplayDevice &other) {
  this->handle = other.GetHandle();
  this->device_name = other.GetName();
  this->device_string = other.GetString();
  this->device_key = other.GetKey();
  this->device_id = other.device_id;
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
  std::string full_id = "";
  while (EnumDisplayDevicesW(device_name, device_index, &dd,
                             EDD_GET_DEVICE_INTERFACE_NAME)) {
    this->device_name = wstringToString(dd.DeviceName);
    this->device_string = wstringToString(dd.DeviceString);
    full_id = wstringToString(dd.DeviceID);
    this->device_key = wstringToString(dd.DeviceKey);
    device_index++;
    // If there are multiple displays to this physical device,
    // keep cycling until the active one is found
    if (dd.StateFlags & DISPLAY_DEVICE_ACTIVE) {
      full_id = wstringToString(dd.DeviceID);
      found_active_device = true;
      break;
    }
  }

  if (full_id == "")
    return;

  this->device_id = std::make_shared<DisplayDeviceId>(full_id);
  if (!found_active_device) {
    spdlog::error("No active device founds for " +
                  wstringToString(device_name));
  }
}

const std::string MonitorSwitch::DisplayDevice::GetName() const {
  return this->device_name;
}

const std::string MonitorSwitch::DisplayDevice::GetString() const {
  return this->device_string;
}

const std::string MonitorSwitch::DisplayDevice::GetKey() const {
  return this->device_string;
}

const std::string MonitorSwitch::DisplayDevice::GetProductCode() const {
  return this->device_id->GetVendorProductCode();
}

const std::string MonitorSwitch::DisplayDevice::GetUId() const {
  return this->device_id->GetUID();
}

const std::string MonitorSwitch::DisplayDevice::GetFullId() const {
  return this->device_id->GetFullIdString();
}

const HMONITOR MonitorSwitch::DisplayDevice::GetHandle() const {
  return this->handle;
}

void MonitorSwitch::DisplayDevice::SetHandle(HMONITOR handle) {
  this->handle = handle;
}

void MonitorSwitch::DisplayDevice::LoadInputs() {
  DWORD currentValue = 0, maxValue = 0;

  if (GetVCPFeatureAndVCPFeatureReply(this->handle, 0x60, NULL, &currentValue,
                                      &maxValue)) {
    std::cout << "Current Input Source: " << currentValue << std::endl;
    std::cout << "Maximum Supported Input Source Value: " << maxValue
              << std::endl;
  } else {
    std::cerr << "Failed to get VCP feature reply for 0x60" << std::endl;
  }
}

std::ostream &
MonitorSwitch::operator<<(std::ostream &os,
                          const MonitorSwitch::DisplayDevice &dd) {
  os << "Display Device: " << dd.GetString()
     << std::endl
     //<< "\tDevice Full ID: " << dd.device_full_id << std::endl
     << "\tDevice Key: " << dd.GetKey() << std::endl
     << "\tDevice Name: " << dd.GetName() << std::endl
     << "\tDevice UId: " << dd.GetUId() << std::endl
     << "\tDevice Product Code: " << dd.GetProductCode() << std::endl
     << "\tDevice Full Id: " << dd.GetFullId() << std::endl
     << "\tDevice Handle: " << dd.GetHandle() << std::endl;

  return os;
}
