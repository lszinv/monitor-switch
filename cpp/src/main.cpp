#include <iostream>
#include "vcp.cpp"
#include "display_device.cpp"
#include "setup_api.cpp"
#include "handle_info.cpp"

int main(int argc, char* argv[]) {
  vcpmain();
  EnumerateDisplayDevices();
  GetMonitorInfo();
  std::cout << "-----------" << std::endl;
  handle_info::GetMonitorDeviceIds();
  handle_info::MatchWithSetupAPI();
  return 0;
}
