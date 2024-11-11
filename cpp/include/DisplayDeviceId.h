#pragma once
#include <string>

namespace MonitorSwitch {
typedef std::string DeviceUId;

class DisplayDeviceId {
public:
  DisplayDeviceId(std::string id);

  std::string GetDisplayClass() const;
  std::string GetVendorProductCode() const;
  std::string GetUID() const;
  std::string GetInterfaceGUID() const;
  std::string GetFullIdString() const;

private:
  std::string full_id_string;
  std::string display_class;
  std::string vendor_product_code;
  DeviceUId uid;
  std::string interface_guid;
};

std::ostream &operator<<(std::ostream &os,
                         const MonitorSwitch::DisplayDeviceId &ddi);
} // namespace MonitorSwitch
