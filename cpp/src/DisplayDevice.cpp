#include "DisplayDevice.h"
#include <iostream>

using namespace MonitorSwitch;

DisplayDevice::DisplayDevice() {
  std::cout << "DisplayDevice" << std::endl;
  return;
}

void DisplayDevice::setPhysicalMonitor(HANDLE physical_monitor) {
  this->physical_monitor = physical_monitor;
}

void DisplayDevice::setDisplayMonitor(HMONITOR display_monitor) {
  this->display_monitor = display_monitor;
}

void DisplayDevice::setName(std::wstring name) {
  this->name = name;
}

void DisplayDevice::setDeviceString(std::wstring device_string) {
  this->device_string = device_string;
}

void DisplayDevice::setDeviceId(std::wstring device_id) {
  this->device_id = device_id;
}

void DisplayDevice::setInputs(std::map<int, std::wstring>&& inputs) {
  this->inputs = std::move(inputs);
}

void DisplayDevice::setCurrentInput(std::pair<int, std::wstring> current_input) {
  this->current_input = current_input;
}
