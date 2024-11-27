#include "DisplayDevice.h"
#include "DisplayDeviceId.h"
#include "spdlog/spdlog.h"
#include <codecvt>
#include <iostream>
#include <locale>
#include <sstream>
#include <string>
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
  // this->LoadInputs();
  std::string cap = this->GetCapabilitiesString();
  this->ParseCapabilitiesString(cap);
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
  // char capabilitiesString[2048] = {0};
  // if (CapabilitiesRequestAndCapabilitiesReply(this->monitor.hPhysicalMonitor, capabilitiesString, sizeof(capabilitiesString))) {
  //   std::cout << "Capabilities String: " << capabilitiesString << std::endl;
  // } else {
  //   DWORD error = GetLastError();
  //   spdlog::error("Failed to get capabilities string. Error code: {}", error);
  // }

  // DWORD currentValue = 0, maxValue = 0;

  // if (GetVCPFeatureAndVCPFeatureReply(this->handle, 0x60, NULL, &currentValue,
  //                                     &maxValue)) {
  //   std::cout << "Current Input Source: " << currentValue << std::endl;
  //   std::cout << "Maximum Supported Input Source Value: " << maxValue
  //             << std::endl;
  // } else {
  //   DWORD error = GetLastError();
  //   spdlog::error("Failed to get VCP feature reply for 0x60. Error code: {}", error);
  // }
}

std::string MonitorSwitch::DisplayDevice::GetCapabilitiesString() const {
  std::vector<std::string> supportedSources;
  std::vector<PHYSICAL_MONITOR> monitors = this->GetPhysicalMonitors();
  // get physical monitors

  DWORD capStrLength = 0;
  if (!GetCapabilitiesStringLength(monitors[0].hPhysicalMonitor, &capStrLength)) {
    spdlog::error("Error: Unable to get capabilities string length. Error code: {}", GetLastError());
    return "";
  }

  std::vector<char> capabilitiesBuffer(capStrLength + 1);
  if (!CapabilitiesRequestAndCapabilitiesReply(monitors[0].hPhysicalMonitor, capabilitiesBuffer.data(), capStrLength)) {
    spdlog::error("Error: Unable to get capabilities string. Error code: {}", GetLastError());
    return "";
  }

  std::string capabilities(capabilitiesBuffer.data());
  spdlog::debug("Capabilities string: {}", capabilities);
  DestroyPhysicalMonitors(1, &monitors[0]);
  return capabilities;

}

std::vector<PHYSICAL_MONITOR> MonitorSwitch::DisplayDevice::GetPhysicalMonitors() const {
  DWORD numPhysicalMonitors = 0;
  if (!GetNumberOfPhysicalMonitorsFromHMONITOR(this->handle, &numPhysicalMonitors)) {
    spdlog::error("Error: Failed to get the number of physical monitors. Error code: {}", GetLastError());
    // figure out if i can return here
  }
  spdlog::info("Num physical monitors: {}", numPhysicalMonitors);
  std::vector<PHYSICAL_MONITOR> physicalMonitors(numPhysicalMonitors);
  if (!GetPhysicalMonitorsFromHMONITOR(this->handle, numPhysicalMonitors, physicalMonitors.data())) {
    spdlog::error("Error: Failed to get physical monitors. Error code: {}",GetLastError());
  }

  // here.
  return physicalMonitors;
}

void MonitorSwitch::DisplayDevice::ParseCapabilitiesString(std::string capabilities) {
  size_t model_pos = capabilities.find("model(");
  if (model_pos == std::string::npos) {
    spdlog::error("Error: Capabilities string had no model1 info.");
    return;
  }
  model_pos += 6; // skip "model(" prefix
  size_t model_pos_end = capabilities.find(")", model_pos);
  std::string model_name = capabilities.substr(model_pos, model_pos_end - model_pos);
  spdlog::info("MODEL: {}", model_name);

  size_t vcp_pos = capabilities.find("vcp(");
  if (vcp_pos == std::string::npos) {
    spdlog::error("Error: Capabilities string had no VCP setting.");
    return;
  }
  size_t vcp_input = capabilities.find("60(", vcp_pos);
  if (vcp_input == std::string::npos) {
    spdlog::error("Could not find VCP code 60.");
    return;
  }
  vcp_input += 3; // "60("
  size_t vcp_input_end = capabilities.find(")", vcp_input);
  std::string vcp_inputs = capabilities.substr(vcp_input, vcp_input_end - vcp_input);
  spdlog::info("VCP Input: {}", vcp_inputs);
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
