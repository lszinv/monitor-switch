#include <iostream>
#include "vcp.cpp"
#include "display_device.cpp"
#include "setup_api.cpp"
#include "handle_info.cpp"
#include "DisplayDevice.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <iostream>
using namespace MonitorSwitch;
using namespace rapidjson;

#include "DisplayDeviceManager.h"

int main(int argc, char* argv[]) {
  //vcpmain(); // vcp.cpp
  EnumerateDisplayDevices(); // display_device
  GetMonitorInfo(); // setup_api
  std::cout << "-----------" << std::endl;
  handle_info::GetMonitorDeviceIds(); // handle_info
  handle_info::MatchWithSetupAPI(); // handle_info
  DisplayDevice dd;
  dd.setDeviceId(L"asdasd");
  DisplayDeviceManager ddm;
  ddm.Scan();
  // 1. Parse a JSON string into DOM.
  std::string json = "{\"project\":\"rapidjson\",\"stars\":10}";
  Document d;
  d.Parse(json.c_str());

  // 2. Modify it by DOM.
  Value& s = d["stars"];
  s.SetInt(s.GetInt() + 1);

  // 3. Stringify the DOM
  StringBuffer buffer;
  Writer<StringBuffer> writer(buffer);
  d.Accept(writer);

  // Output {"project":"rapidjson","stars":11}
  std::cout << buffer.GetString() << std::endl;
  return 0;
}
