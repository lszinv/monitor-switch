#include <windows.h>
#include <setupapi.h>
#include <cfgmgr32.h>
#include <iostream>
#include <initguid.h>
#include <devguid.h> // For GUID_DEVCLASS_MONITOR

#pragma comment(lib, "setupapi.lib")

void GetMonitorInfo() {
  // Define GUID_DEVCLASS_MONITOR if not already defined

  #ifndef GUID_DEVCLASS_MONITOR
const GUID GUID_DEVCLASS_MONITOR =
    {0x4d36e96e, 0xe325, 0x11ce, {0xbf, 0xc1, 0x08, 0x00, 0x2b, 0xe1, 0x03, 0x18}};
#endif

  HDEVINFO deviceInfoSet = SetupDiGetClassDevs(&GUID_DEVCLASS_MONITOR, NULL, NULL, DIGCF_PRESENT);

  if (deviceInfoSet == INVALID_HANDLE_VALUE) {
    std::cerr << "Failed to get device info set" << std::endl;
    return;
  }

  SP_DEVINFO_DATA deviceInfoData;
  deviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

  for (DWORD i = 0; SetupDiEnumDeviceInfo(deviceInfoSet, i, &deviceInfoData); i++) {
    CHAR deviceID[MAX_DEVICE_ID_LEN];
    if (CM_Get_Device_ID(deviceInfoData.DevInst, deviceID, MAX_DEVICE_ID_LEN, 0) == CR_SUCCESS) {
      std::cout << "Monitor Device ID: " << deviceID << std::endl;
    }
  }

  SetupDiDestroyDeviceInfoList(deviceInfoSet);
}
