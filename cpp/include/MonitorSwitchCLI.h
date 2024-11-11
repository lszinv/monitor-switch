#pragma once
#include <algorithm>
#include "CLI11.hpp"
#include <iostream>
#include <string>

namespace MonitorSwitch {
  class MonitorSwitchCLI {
  public:
    MonitorSwitchCLI();
    bool Parse(int argc, char** argv);
    void Run() const;
  private:
    CLI::App app;
    std::string monitor_name;
    std::string input_code;
    bool scan;

    CLI::App* get_monitors_cmd;
    CLI::App* get_monitor_inputs_cmd;
    CLI::App* change_input_cmd;
  };
}
