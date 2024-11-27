#include "ConfigManager.h"
#include "DisplayDevice.h"
#include "display_device.cpp"
#include "handle_info.cpp"
#include "setup_api.cpp"
#include "spdlog/common.h"
#include "vcp.cpp"
#include <iostream>

#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include <iostream>
#include "spdlog/spdlog.h"

using namespace MonitorSwitch;
using namespace rapidjson;

#include "DisplayDeviceManager.h"

int main(int argc, char *argv[]) {
  spdlog::set_level(spdlog::level::level_enum::debug);
  spdlog::info("App Started");
  // spdlog::info("===EnumerateDisplayDevices");
  // EnumerateDisplayDevices(); // display_device
  // spdlog::info("===GetMonitorInfo()");
  // GetMonitorInfo();          // setup_api
  // spdlog::info("GetMonitordeviceIds / MatchWithSetupAPI");
  // handle_info::GetMonitorDeviceIds(); // handle_info
  // handle_info::MatchWithSetupAPI();   // handle_info

  DisplayDeviceManager dm;
  dm.Scan();
  spdlog::info(dm.GetDevices().size());
  for (const auto& [key, dd] : dm.GetDevices()) {
    std::cout << dd << std::endl;
  }

  // ConfigManager cm;
  // cm.SaveConfig(dm, "monitors.json");
  return 0;
}
