#include <cfgmgr32.h>
#include <dxgi.h>
#include <iostream>
#include <map>
#include <setupapi.h>
#include <string>
#include <vector>
#include <windows.h>

#pragma comment(lib, "setupapi.lib")

namespace handle_info {
struct MonitorInfo {
  HMONITOR hMonitor;
  std::wstring deviceName;
  std::wstring deviceId;
};

std::vector<MonitorInfo> monitorInfos;

BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor,
                              LPRECT lprcMonitor, LPARAM dwData) {
  MONITORINFOEXW mi;
  mi.cbSize = sizeof(MONITORINFOEXW);
  if (GetMonitorInfoW(hMonitor, &mi)) {
    MonitorInfo info;
    info.hMonitor = hMonitor;
    info.deviceName = mi.szDevice;
    monitorInfos.push_back(info);
  }
  return TRUE;
}

void GetMonitorDeviceIds() {
  EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, 0);

  std::wcout << L"Found " << monitorInfos.size() << L" monitors.\n\n";

  for (auto &info : monitorInfos) {
    std::wcout << L"Monitor Handle: " << info.hMonitor << L"\n";
    std::wcout << L"Device Name: " << info.deviceName << L"\n";

    DISPLAY_DEVICEW dd = {sizeof(dd)};
    DWORD deviceIndex = 0;
    bool found = false;

    while (EnumDisplayDevicesW(info.deviceName.c_str(), deviceIndex, &dd,
                               EDD_GET_DEVICE_INTERFACE_NAME)) {
      std::wcout << L"  Checking device index " << deviceIndex << L":\n";
      std::wcout << L"    Device String: " << dd.DeviceString << L"\n";
      std::wcout << L"    Device ID: " << dd.DeviceID << L"\n";
      std::wcout << L"    Device Key: " << dd.DeviceKey << L"\n";
      std::wcout << L"    State Flags: 0x" << std::hex << dd.StateFlags
                 << std::dec << L"\n";

      if (dd.StateFlags & DISPLAY_DEVICE_ACTIVE) {
        info.deviceId = dd.DeviceID;
        std::wcout << L"    This is the active device. Setting as monitor's "
                      L"Device ID.\n";
        found = true;
        break;
      }
      deviceIndex++;
    }

    if (found) {
      std::wcout << L"Final Device ID for this monitor: " << info.deviceId
                 << L"\n\n";
    } else {
      std::wcout << L"No active device found for this monitor.\n\n";
    }
  }
}

// void GetMonitorDeviceIds() {
//   EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, 0);

//   for (auto& info : monitorInfos) {
//     DISPLAY_DEVICEW dd = { sizeof(dd) };
//     DWORD deviceIndex = 0;
//     while (EnumDisplayDevicesW(info.deviceName.c_str(), deviceIndex, &dd,
//     EDD_GET_DEVICE_INTERFACE_NAME)) {
//       if (dd.StateFlags & DISPLAY_DEVICE_ACTIVE) {
//         info.deviceId = dd.DeviceID;
//         break;
//       }
//       deviceIndex++;
//     }
//   }
// }

void MatchWithSetupAPI() {
  const GUID GUID_DEVINTERFACE_MONITOR = {
      0xe6f07b5f,
      0xee97,
      0x4a90,
      {0xb0, 0x76, 0x33, 0xf5, 0x7b, 0xf4, 0xea, 0xa7}};
  HDEVINFO deviceInfoSet =
      SetupDiGetClassDevs(&GUID_DEVINTERFACE_MONITOR, NULL, NULL,
                          DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);

  if (deviceInfoSet == INVALID_HANDLE_VALUE) {
    return;
  }

  SP_DEVICE_INTERFACE_DATA interfaceData;
  interfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

  for (DWORD i = 0; SetupDiEnumDeviceInterfaces(
           deviceInfoSet, NULL, &GUID_DEVINTERFACE_MONITOR, i, &interfaceData);
       i++) {
    DWORD requiredSize = 0;
    SetupDiGetDeviceInterfaceDetail(deviceInfoSet, &interfaceData, NULL, 0,
                                    &requiredSize, NULL);

    std::vector<BYTE> detailDataBuffer(requiredSize);
    PSP_DEVICE_INTERFACE_DETAIL_DATA pDetailData =
        reinterpret_cast<PSP_DEVICE_INTERFACE_DETAIL_DATA>(
            &detailDataBuffer[0]);
    pDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

    if (SetupDiGetDeviceInterfaceDetail(deviceInfoSet, &interfaceData,
                                        pDetailData, requiredSize, NULL,
                                        NULL)) {

      int length =
          MultiByteToWideChar(CP_ACP, 0, pDetailData->DevicePath, -1, NULL, 0);
      std::wstring setupApiDeviceId(length, L'\0');
      MultiByteToWideChar(CP_ACP, 0, pDetailData->DevicePath, -1,
                          &setupApiDeviceId[0], length);

      // Match with EnumDisplayDevices ID
      for (auto &info : monitorInfos) {
        if (_wcsicmp(info.deviceId.c_str(), setupApiDeviceId.c_str()) == 0) {
          std::wcout << L"Matched HMONITOR " << info.hMonitor
                     << L" with SetupAPI device: " << setupApiDeviceId
                     << std::endl;
          break;
        }
      }
    }
  }

  SetupDiDestroyDeviceInfoList(deviceInfoSet);
}
} // namespace handle_info
