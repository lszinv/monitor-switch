#include "DisplayDeviceManager.h"
#include "DisplayDevice.h"
#include "DisplayDeviceId.h"
#include "spdlog/spdlog.h"
#include <iostream>
#include <codecvt>
#include <locale>
#include <sstream>
#include <string>

using namespace MonitorSwitch;

std::vector<DisplayDevice> DisplayDeviceManager::display_devices = {};

DisplayDeviceManager::DisplayDeviceManager() {
  this->devices = {};
  return;
};

void DisplayDeviceManager::Scan() {
  spdlog::debug("Scanning");
  this->ResetHandles();
  EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, reinterpret_cast<LPARAM>(this));
  return;
}

BOOL CALLBACK DisplayDeviceManager::MonitorEnumProc(HMONITOR hMonitor,
                                                    HDC hdcMonitor,
                                                    LPRECT lprcMonitor,
                                                    LPARAM dwData) {
  MONITORINFOEXW mi;
  mi.cbSize = sizeof(MONITORINFOEXW);
  // Retrieve class pointer back from passed context
  auto* pThis = reinterpret_cast<DisplayDeviceManager*>(dwData);
  if (GetMonitorInfoW(hMonitor, &mi)) {

#ifdef DEBUG
    std::stringstream ss;
    ss << "Found: " << hMonitor;
    spdlog::debug(ss.str());
#endif

    DisplayDevice dd(hMonitor, mi.szDevice);
    pThis->UpdateDevice(std::move(dd));
  }
  return TRUE;
}

void DisplayDeviceManager::UpdateDevice(DisplayDevice&& device) {
  if (this->devices.count(device.GetUId()) > 0) {
    spdlog::debug("Found existing device entry. Updating handle.");
    this->devices[device.GetUId()].SetHandle(device.GetHandle());
    return;
  }
  spdlog::debug("Did not find existing entry. Adding new entry.");
  this->devices[device.GetUId()] = std::move(device);
}

void DisplayDeviceManager::ResetHandles() {
  for (auto& [uid, d] : DisplayDeviceManager::devices) {
    d.SetHandle(NULL);
  }
}

DisplayDevice *DisplayDeviceManager::GetDisplayDevice(DeviceUId device_id) {
  if (this->devices.count(device_id) == 0) {
    spdlog::info("Attempted to retrieve a display that is not managed: " + device_id);
    return nullptr;
  }
  return &this->devices[device_id];
}

const std::map<DeviceUId, DisplayDevice>& DisplayDeviceManager::GetDevices() const {
  return this->devices;
}
