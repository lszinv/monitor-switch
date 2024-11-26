#pragma once
#include <map>
#include <memory>
#include <string>
#include <windows.h>
#include <iostream>
#include "DisplayDeviceId.h"

namespace MonitorSwitch {

  class DisplayDevice {
  public:
    DisplayDevice() = default;
    DisplayDevice(HMONITOR monitor, std::wstring device_name);
    DisplayDevice(const DisplayDevice& other);

    void SetInputs(std::map<int, std::string> &&);
    void SetCurrentInput(std::pair<int, std::string>);

    void SetHandle(HMONITOR handle);
    const HMONITOR GetHandle() const;
    const std::string GetName() const;
    const std::string GetString() const;
    const std::string GetKey() const;
    const std::string GetProductCode() const;
    const std::string GetUId() const;
    const std::string GetFullId() const;


  private:
    // TODO: typedef these
    std::map<int /* Code */, std::string /* Input Label  */> inputs;
    std::pair<int /* Code */, std::string /* Inputn Label */> current_input;
    HMONITOR handle;
    std::string device_name;
    std::string device_string;
    std::string device_key;
    std::shared_ptr<DisplayDeviceId> device_id;

    void GetMonitorDetails(LPCWSTR device_name);
    void ParseId(std::string id);
    void LoadInputs();
    std::string wstringToString(const std::wstring& wstr);
  };

  std::ostream& operator<<(std::ostream& os, const MonitorSwitch::DisplayDevice& dd);
} // namespace MonitorSwitch
