#include "MonitorSwitchCLI.h"
using namespace MonitorSwitch;

int main(int argc, char *argv[]) {
  MonitorSwitchCLI cli;
  cli.Parse(argc, argv);
  cli.Run();
  return 0;
}
