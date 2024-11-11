# monitor-switch
Windows App to switch Monitor inputs via CLI.

- Only works on Windows as interaction to the [MCCS](https://en.wikipedia.org/wiki/Monitor_Control_Command_Set) protocol through VCP through WinAPI.
- Similarly functionality can be achieved with [DDCUtil](https://www.ddcutil.com/) in Linux.
- MacOS has [ddcctl](https://github.com/kfix/ddcctl)


# Build Dependencies
Building in WSL
```
sudo apt update
sudo apt install mingw-w64 mingw-w64-tools mingw-w64-common
cd monitor-switch
mkdir -p ./cpp/build
cd ./cpp/build
cmake ..
make
```

# Outputs
Multiple outputs are generated from the CMake:
1. **MonitorSwitchCLI.exe** - The CLI based application to query for monitor ids and change inputs.
2. **libMonitorSwitch.dll** - Shared lib to access monitor switching functionality. Intended for use with a to-do GUI.
3. **DedicatedMonitorSwitch.exe** - Hard-coded dedicated monitor switch app if it makes hotkeying any easier to have.
