#include <highlevelmonitorconfigurationapi.h>
#include <iostream>
#include <lowlevelmonitorconfigurationapi.h>
#include <memory>
#include <physicalmonitorenumerationapi.h>
#include <vector>
#include <windows.h>

class MonitorHandle {
private:
  HANDLE handle;

public:
  MonitorHandle(HANDLE h) : handle(h) {}
  ~MonitorHandle() {
    if (handle) {
      DestroyPhysicalMonitor(handle);
    }
  }
  HANDLE get() const { return handle; }
};

class MonitorInfo {
public:
  std::wstring name;
  HANDLE physicalMonitor;

  MonitorInfo(const std::wstring &n, HANDLE h) : name(n), physicalMonitor(h) {}
};

std::vector<MonitorInfo> getPhysicalMonitors() {
  std::vector<MonitorInfo> monitors;

  auto enumMonitors = [](HMONITOR handle, HDC dc, LPRECT rect,
                         LPARAM param) -> BOOL {
    auto &monitors = *reinterpret_cast<std::vector<MonitorInfo> *>(param);

    MONITORINFOEXW monitorInfo{};
    monitorInfo.cbSize = sizeof(MONITORINFOEXW);
    if (!GetMonitorInfoW(handle, &monitorInfo)) {
      return TRUE;
    }

    DWORD numPhysicalMonitors;
    if (!GetNumberOfPhysicalMonitorsFromHMONITOR(handle,
                                                 &numPhysicalMonitors)) {
      return TRUE;
    }

    std::vector<PHYSICAL_MONITOR> physicalMonitors(numPhysicalMonitors);
    if (!GetPhysicalMonitorsFromHMONITOR(handle, numPhysicalMonitors,
                                         physicalMonitors.data())) {
      return TRUE;
    }

    for (const auto &pm : physicalMonitors) {
      monitors.emplace_back(pm.szPhysicalMonitorDescription,
                            pm.hPhysicalMonitor);
    }

    return TRUE;
  };

  EnumDisplayMonitors(nullptr, nullptr, enumMonitors,
                      reinterpret_cast<LPARAM>(&monitors));
  return monitors;
}

void getVCPFeatures(const MonitorInfo &monitor) {
  std::wcout << L"Monitor: " << monitor.name << std::endl;

  // Common VCP codes to check
  // const BYTE vcpCodes[] = {
  //   0x1E, // Serial Number
  //   0x10, // Brightness
  //   0x12, // Contrast
  //   0x62, // Audio volume
  //   0xB6  // Display technology type
  // };

  std::vector<BYTE> vcpCodes = {};
  for (BYTE a = 0x00; a <= 0xB6; a++) {
    vcpCodes.push_back(a);
  }

  for (BYTE code : vcpCodes) {
    DWORD currentValue = 0;
    DWORD maximumValue = 0;

    if (GetVCPFeatureAndVCPFeatureReply(monitor.physicalMonitor, code, nullptr,
                                        &currentValue, &maximumValue)) {
      std::wcout << L"VCP Code 0x" << std::hex << static_cast<int>(code)
                 << L": Current=" << std::dec << currentValue << L", Maximum="
                 << maximumValue << std::endl;
    }
  }
  std::wcout << L"-------------------" << std::endl;
}

int vcpmain() {
  auto monitors = getPhysicalMonitors();

  if (monitors.empty()) {
    std::wcout << L"No monitors found!" << std::endl;
    return 1;
  }

  for (const auto &monitor : monitors) {
    getVCPFeatures(monitor);
  }

  return 0;
}
