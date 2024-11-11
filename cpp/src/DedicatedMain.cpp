#include "DisplayDeviceManager.h"
#include "spdlog/common.h"
#include "spdlog/spdlog.h"
#include <memory>
#include <iostream>
using namespace MonitorSwitch;

/**
   For an application dedicated to a single switch to be loaded into StreamDeck.
   TODO: Create a service to receive inputs from Windows Pipe, and make a StreamDeck node plugin to send pipe commands.
 */
int main(int argc, char *argv[]) {
  std::shared_ptr<DisplayDeviceManager> dm = DisplayDeviceManager::GetDisplayDeviceManager();

  dm->Scan();
  spdlog::info(dm->GetDevices().size());
  for (auto& [key, dd] : dm->GetDevices()) {
    std::cout << key << ": " << dd << std::endl;
    if (dd.GetUId() == "7&1cc41224&0&UID20737") {
      //dd.ChangeInput(InputType::DISPLAY_PORT_2);
      dd.ChangeInput(InputType::DISPLAY_PORT_1);
    }
  }
}
