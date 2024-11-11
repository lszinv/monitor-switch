#pragma once

/**
   TODO: Add output arg for error messages, set return value to be success bool
 */
extern "C" {
  __declspec(dllexport) bool SetMonitorInput(std::string id, int input);
  __declspec(dllexport) const char** GetMonitors();
  __declspec(dllexport) bool GetMonitorInfo(std::string id, int* count);
}
