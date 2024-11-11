#pragma once
#include "mccs/base/InputSource.h"
#include <map>

namespace MonitorSwitch {
typedef int InputCode;
class Mccs2_2aInput : public BaseInput {
  static const std::map<InputCode, InputType> input_select;
};
} // namespace MonitorSwitch
