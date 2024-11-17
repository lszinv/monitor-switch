#include "ConfigManager.h"
#include "DisplayDevice.h"
#include "display_device.cpp"
#include "handle_info.cpp"
#include "setup_api.cpp"
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
  spdlog::info("App Started");
  DisplayDeviceManager dm;
  dm.Scan();
  DisplayDevice dd = dm.display_devices[0];
  std::cout << dd  << std::endl;
  ConfigManager cm;
  cm.SaveConfig(dm, "monitors.json");
  return 0;
}
