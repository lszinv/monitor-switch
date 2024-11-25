#include "ConfigManager.h"
#include "DisplayDevice.h"
#include "rapidjson/document.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "spdlog/spdlog.h"
#include <iostream>
#include <fstream>

using namespace MonitorSwitch;
using namespace rapidjson;

ConfigManager::ConfigManager() {}

void ConfigManager::SaveConfig(DisplayDeviceManager display_device_manager, std::string path) {
  Document d;
  d.SetObject();
  Document::AllocatorType& allocator = d.GetAllocator();
  Value displays(kArrayType);

  for (DisplayDevice dd : display_device_manager.display_devices) {
    // Value display_device(kObjectType);
    // Value device_string;
    // device_string.SetString(dd.device_string.c_str(), allocator);
    // display_device.AddMember("device_string", device_string, allocator);
    // Value device_full_id;
    // device_full_id.SetString(dd.device_full_id.c_str(), allocator);
    // display_device.AddMember("device_full_id", device_full_id, allocator);
    // Value device_key;
    // device_key.SetString(dd.device_key.c_str(), allocator);
    // display_device.AddMember("device_key", device_key, allocator);
    // displays.PushBack(display_device, allocator);
  }

  d.AddMember("displays", displays, allocator);
  StringBuffer sb;
  rapidjson::Writer<rapidjson::StringBuffer> writer(sb);
  d.Accept(writer);

  std::ofstream file(path);
   if (file.is_open()) {
        file << sb.GetString();
        file.close();
        spdlog::info("Config saved to [" + path + "]");
    } else {
        std::cerr << "Unable to open file for writing" << std::endl;
    }

}

DisplayDeviceManager LoadConfig(std::string path) {
  DisplayDeviceManager dm;
  return dm;
}
