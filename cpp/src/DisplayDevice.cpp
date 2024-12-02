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
  std::string cap = this->GetCapabilitiesString();
  this->ParseCapabilitiesString(cap);
  return;
}

DisplayDevice::DisplayDevice(const DisplayDevice &other) {
  this->handle = other.GetHandle();
  this->model_name = other.GetModelName();
  this->device_name = other.GetName();
  this->device_string = other.GetString();
  this->device_key = other.GetKey();
  this->device_id = other.device_id;
}

DisplayDevice::~DisplayDevice() {
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

const std::string DisplayDevice::GetName() const {
  return this->device_name;
}

const std::string DisplayDevice::GetString() const {
  return this->device_string;
}

const std::string DisplayDevice::GetKey() const {
  return this->device_string;
}

const std::string DisplayDevice::GetProductCode() const {
  return this->device_id->GetVendorProductCode();
}

const std::string DisplayDevice::GetUId() const {
  return this->device_id->GetUID();
}

const std::string DisplayDevice::GetFullId() const {
  return this->device_id->GetFullIdString();
}

const HMONITOR DisplayDevice::GetHandle() const {
  return this->handle;
}

const std::string DisplayDevice::GetModelName() const {
  return this->model_name;
}

void MonitorSwitch::DisplayDevice::SetHandle(HMONITOR handle) {
  this->handle = handle;
}

std::string DisplayDevice::GetCapabilitiesString() {
  // todo: break this up to smaller functions that fit the name

  std::vector<std::string> supportedSources;
  std::vector<PHYSICAL_MONITOR> physical_monitors = this->GetPhysicalMonitors();
  // get physical monitors
  DWORD capStrLength = 0;
  if (!GetCapabilitiesStringLength( physical_monitors[0].hPhysicalMonitor,
                                    &capStrLength)) {
    spdlog::error("Error: Unable to get capabilities string length. Error code: {}", GetLastError());
    return "";
  }
  std::vector<char> capabilitiesBuffer(capStrLength + 1);
  if (!CapabilitiesRequestAndCapabilitiesReply( physical_monitors[0].hPhysicalMonitor,
                                                capabilitiesBuffer.data(),
                                                capStrLength)) {
    spdlog::error("Error: Unable to get capabilities string. Error code: {}", GetLastError());
    return "";
  }

  std::string capabilities(capabilitiesBuffer.data());
  spdlog::debug("Capabilities string: {}", capabilities);
  this->CleanUpPhysicalMonitors(std::move(physical_monitors));
  return capabilities;

}

std::vector<PHYSICAL_MONITOR> MonitorSwitch::DisplayDevice::GetPhysicalMonitors() const {
  DWORD numPhysicalMonitors = 0;
  if (!GetNumberOfPhysicalMonitorsFromHMONITOR(this->handle, &numPhysicalMonitors)) {
    spdlog::error("Error: Failed to get the number of physical monitors. Error code: {}", GetLastError());
    // figure out if i can return here
  }
  std::vector<PHYSICAL_MONITOR> physicalMonitors(numPhysicalMonitors);
  if (!GetPhysicalMonitorsFromHMONITOR(this->handle, numPhysicalMonitors, physicalMonitors.data())) {
    spdlog::error("Error: Failed to get physical monitors. Error code: {}",GetLastError());
  }

  return physicalMonitors;
}

void MonitorSwitch::DisplayDevice::ParseCapabilitiesString(std::string capabilities) {
  size_t model_pos = capabilities.find("model(");
  if (model_pos == std::string::npos) {
    spdlog::error("Error: Capabilities string had no model info.");
    return;
  }
  model_pos += 6; // skip "model(" prefix
  size_t model_pos_end = capabilities.find(")", model_pos);
  this->model_name = capabilities.substr(model_pos, model_pos_end - model_pos);
  spdlog::info("MODEL: {}", this->model_name);

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
  this->ParseInputs(vcp_inputs);
  spdlog::info("VCP Input: {}", vcp_inputs);
}

void MonitorSwitch::DisplayDevice::ParseInputs(std::string inputs) {
  spdlog::info("Parsing string for inputs: {}", inputs);
  std::stringstream ss(inputs);
  std::string token;

  while (ss >> token) {
    try {
      // Parse hex to int
      int value = std::stoi(token, nullptr, 16);

      // Skip values not in defined MCCS range
      if (value >= InputType::UNSUPPORTED || value < InputType::ANALOG_1) {
        continue;
      }

      InputType t = static_cast<InputType>(value);
      this->inputs.insert(t);
    } catch (const std::invalid_argument& e) {
      spdlog::error("Received an invalid input hex: {}", token);
    } catch (const std::out_of_range& e) {
      spdlog::error("Received an out of range input hex value: {}", token);
    }
  }
}

void MonitorSwitch::DisplayDevice::ChangeInput(int input) const {
  std::vector<PHYSICAL_MONITOR> physical_monitors = this->GetPhysicalMonitors();
  SetVCPFeature(physical_monitors[0].hPhysicalMonitor, 0x60, input);
  this->CleanUpPhysicalMonitors(std::move(physical_monitors));
}

void MonitorSwitch::DisplayDevice::ChangeInput(InputType input) const {
  spdlog::info("Changing [{}] Input to: {}", this->model_name, InputTypeToString.at(input));
  int input_value = static_cast<int>(input);
  this->ChangeInput(input_value);
}

InputType MonitorSwitch::DisplayDevice::GetCurrentInput() const {
  return InputType::UNSUPPORTED;
}

void DisplayDevice::CleanUpPhysicalMonitors(std::vector<PHYSICAL_MONITOR>&& physical_monitors) const {
  for (const auto& mon : physical_monitors) {
    DestroyPhysicalMonitor(mon.hPhysicalMonitor);
  }
}

std::ostream &
MonitorSwitch::operator<<(std::ostream &os,
                          const MonitorSwitch::DisplayDevice &dd) {
  os << "Display Device: " << dd.GetString()
     << std::endl
     << "\tDevice Model Name: " << dd.GetModelName() << std::endl
     << "\tDevice Key: " << dd.GetKey() << std::endl
     << "\tDevice Name: " << dd.GetName() << std::endl
     << "\tDevice UId: " << dd.GetUId() << std::endl
     << "\tDevice Product Code: " << dd.GetProductCode() << std::endl
     << "\tDevice Full Id: " << dd.GetFullId() << std::endl
     << "\tDevice Handle: " << dd.GetHandle() << std::endl;

  return os;
}
