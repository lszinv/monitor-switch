#include "DisplayDeviceManager.h"
#include "DisplayDevice.h"
#include <iostream>

using namespace MonitorSwitch;

DisplayDeviceManager::DisplayDeviceManager() {
  return;
};


void DisplayDeviceManager::Scan(){
  std::cout << "??" << std::endl;
  return;
}
DisplayDevice* DisplayDeviceManager::GetDisplayDevice(std::wstring device_id) {
  return &(this->display_devices[0]);
}
