#pragma once
#include <map>
#include <string>

namespace MonitorSwitch {
  enum InputType: int {
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
    UNSUPPORTED = 0x13,
  };

  inline const std::map<InputType, std::string> InputTypeToString = {
    {InputType::ANALOG_1, "ANALOG_1"},
    {InputType::ANALOG_2, "ANALOG_2"},
    {InputType::DVI_1, "DVI_1"},
    {InputType::DVI_2, "DVI_2"},
    {InputType::COMPOSITE_1, "COMPOSITE_1"},
    {InputType::COMPOSITE_2, "COMPOSITE_2"},
    {InputType::SVIDEO_1, "SVIDEO_1"},
    {InputType::SVIDEO_2, "SVIDEO_2"},
    {InputType::TUNER_1, "TUNER_1"},
    {InputType::TUNER_2, "TUNER_2"},
    {InputType::TUNER_3, "TUNER_3"},
    {InputType::COMPONENT_1, "COMPONENT_1"},
    {InputType::COMPONENT_2, "COMPONENT_2"},
    {InputType::COMPONENT_3, "COMPONENT_3"},
    {InputType::DISPLAY_PORT_1, "DISPLAY_PORT_1"},
    {InputType::DISPLAY_PORT_2, "DISPLAY_PORT_2"},
    {InputType::HDMI_1, "HDMI_1"},
    {InputType::HDMI_2, "HDMI_2"},
    {InputType::UNSUPPORTED, "UNSUPPORTED"}
  };

  class BaseInput {
  public:
  };
} // namespace MonitorSwitch
