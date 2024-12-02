#pragma once
#include "DisplayDeviceId.h"
#include "mccs/base/InputSource.h"
#include <PhysicalMonitorEnumerationAPI.h>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>
#include <windows.h>

namespace MonitorSwitch {

  class DisplayDevice {
  public:
    DisplayDevice() = default;
    DisplayDevice(HMONITOR monitor, std::wstring device_name);
    DisplayDevice(const DisplayDevice& other);

    ~DisplayDevice();

    void SetHandle(HMONITOR handle);
    const HMONITOR GetHandle() const;
    const std::string GetName() const;
    const std::string GetString() const;
    const std::string GetKey() const;
    const std::string GetProductCode() const;
    const std::string GetUId() const;
    const std::string GetFullId() const;
    const std::string GetModelName() const;
    void ChangeInput(InputType input) const;
    void ChangeInput(int input) const;
    InputType GetCurrentInput() const;


  private:
    std::set<InputType> inputs;
    InputType current_input;
    HMONITOR handle;
    std::string device_name;
    std::string device_string;
    std::string device_key;
    std::string model_name;
    std::shared_ptr<DisplayDeviceId> device_id;

    void GetMonitorDetails(LPCWSTR device_name);
    void ParseId(std::string id);
    std::vector<PHYSICAL_MONITOR> GetPhysicalMonitors() const;
    std::string GetCapabilitiesString();
    void ParseCapabilitiesString(std::string);
    void ParseInputs(std::string);
    void CleanUpPhysicalMonitors(std::vector<PHYSICAL_MONITOR>&& physical_monitors) const;

    std::string wstringToString(const std::wstring& wstr);
  };

  std::ostream& operator<<(std::ostream& os, const MonitorSwitch::DisplayDevice& dd);
} // namespace MonitorSwitch
