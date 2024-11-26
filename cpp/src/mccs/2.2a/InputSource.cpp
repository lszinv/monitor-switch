#include "mccs/2.2a/InputSource.h"
#include "mccs/base/InputSource.h"
#include <map>

using namespace MonitorSwitch;

const std::map<InputCode, InputType> Mccs2_2aInput::input_select = {
  { 0x01, ANALOG_1 },
  { 0x02, ANALOG_2 },
  { 0x03, DVI_1 },
  { 0x04, DVI_2 },
  { 0x05, COMPOSITE_1 },
  { 0x06, COMPOSITE_2 },
  { 0x07, SVIDEO_1 },
  { 0x08, SVIDEO_2 },
  { 0x09, TUNER_1 },
  { 0x0A, TUNER_2 },
  { 0x0B, TUNER_3 },
  { 0x0C, COMPONENT_1 },
  { 0x0D, COMPONENT_2 },
  { 0x0E, COMPONENT_3 },
  { 0x0F, DISPLAY_PORT_1 },
  { 0x10, DISPLAY_PORT_2 },
  { 0x11, HDMI_1 },
  { 0x12, HDMI_2 },
};
