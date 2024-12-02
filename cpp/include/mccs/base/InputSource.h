#pragma once

namespace MonitorSwitch {
enum InputType {
  ANALOG_1 = 0x01,
  ANALOG_2 = 0x02,
  DVI_1 = 0x03,
  DVI_2 = 0x04,
  COMPOSITE_1 = 0x05,
  COMPOSITE_2 = 0x06,
  SVIDEO_1 = 0x07,
  SVIDEO_2 = 0x08,
  TUNER_1 = 0x09,
  TUNER_2 = 0x0A,
  TUNER_3 = 0x0B,
  COMPONENT_1 = 0x0C,
  COMPONENT_2 = 0x0D,
  COMPONENT_3 = 0x0E,
  DISPLAY_PORT_1 = 0x0F,
  DISPLAY_PORT_2 = 0x10,
  HDMI_1 = 0x11,
  HDMI_2 = 0x12,
};

class BaseInput {
public:
};
} // namespace MonitorSwitch
