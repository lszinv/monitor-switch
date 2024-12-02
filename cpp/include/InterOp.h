#pragma once

extern "C" {
  __declspec(dllexport) void SetMonitorInput(std::string id, int input);
  __declspec(dllexport) const char** GetMonitors();
  __declspec(dllexport) void GetMonitorInfo(std::string id, int* count);
}
