#include "MonitorSwitchCLI.h"
#include "DisplayDeviceManager.h"
#include "CLI11.hpp"
#include "spdlog/spdlog.h"
#include <iostream>

using namespace MonitorSwitch;

MonitorSwitchCLI::MonitorSwitchCLI(): app("My App") {
  app.require_subcommand(1,1);

  // Get monitors
  get_monitors_cmd = app.add_subcommand("get-monitors", "Display a list of monitors");

  // Get inputs
  get_monitor_inputs_cmd = app.add_subcommand("get-inputs", "Display a list of inputs for a monitor");
  get_monitor_inputs_cmd->add_option("-n,--name", monitor_name, "Monitor name(s). Multiple names can be supplied by separating with a comma(,).")->required();

  // Change input
  change_input_cmd = app.add_subcommand("change-input", "Change the input source of a specific monitor");
  change_input_cmd->add_option("-n,--name", monitor_name, "Monitor name(s). Multiple names can be supplied by separating with a comma(,).")->required();
  change_input_cmd->add_option("-t,--target", input_code, "Target monitor input(s). Multiple inputs can be supplied by seprating with a comma(,).")->required();
}

bool MonitorSwitchCLI::Parse(int argc, char** argv) {
  try {
    CLI11_PARSE(app, argc, argv);
    return true;  // Parsing successful
  } catch(const CLI::ParseError &e) {
    return app.exit(e) == 0;
  }
}

void MonitorSwitchCLI::Run() const {
  DisplayDeviceManager manager;
  manager.Scan();
  auto devices = manager.GetDevices();
  DisplayDevice* dd = nullptr;
  if (*get_monitors_cmd) {
    for (auto& [key, device] : devices) {
      std::cout << "Device ID: " << key << " \t"
                << "Product Code: " << device.GetProductCode() << " \t"
                << "Device Name: " << device.GetString()
                << std::endl;
    }
  }

  if (*get_monitor_inputs_cmd) {
    for (auto& [key, device] : devices) {
      if (key == this->monitor_name) {
        dd = &device;
        break;
      }
    }
    if (dd == nullptr) {
      std::cout << "No device found with id: " << this->monitor_name << std::endl;
      return;
    }

    std::cout << "[" << dd->GetUId() << "] "
              << dd->GetString() << " - "
              << dd->GetInputString()
              << std::endl;

  }

  if (*change_input_cmd) {
    for (auto& [key, device] : devices) {
      if (key == this->monitor_name) {
        dd = &device;
        break;
      }
    }
    if (dd == nullptr) {
      std::cout << "No device found with id: " << this->monitor_name << std::endl;
      return;
    }
    int converted_code = std::atoi(this->input_code.c_str());
    dd->ChangeInput(converted_code);

  }
}
