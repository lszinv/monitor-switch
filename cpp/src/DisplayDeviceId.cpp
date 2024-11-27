#include "DisplayDeviceId.h"
#include "spdlog/spdlog.h"
#include <sstream>
#include <stdexcept>
#include <vector>

using namespace MonitorSwitch;

DisplayDeviceId::DisplayDeviceId(std::string id) {
  std::vector<std::string> result;
  std::istringstream stream(id);
  std::string token;
  while (std::getline(stream, token, '#')) {
    result.push_back(token);
  }

  if (result.size() != 4) {
    throw std::runtime_error("Received an unexpected device id: " + id);
  }
  this->full_id_string = id;
  this->display_class = result[0];
  this->vendor_product_code = result[1];
  this->uid = result[2];
  this->interface_guid = result[3];
}

std::string DisplayDeviceId::GetDisplayClass() const {
  return this->display_class;
}

std::string DisplayDeviceId::GetVendorProductCode() const {
  return this->vendor_product_code;
}

std::string DisplayDeviceId::GetUID() const { return this->uid; }

std::string DisplayDeviceId::GetInterfaceGUID() const {
  return this->interface_guid;
}

std::string DisplayDeviceId::GetFullIdString() const {
  return this->full_id_string;
}

std::ostream &
MonitorSwitch::operator<<(std::ostream &os,
                          const MonitorSwitch::DisplayDeviceId &ddi) {
  os << "Display Device Id:" << std::endl
     << "Class: " << ddi.GetDisplayClass() << std::endl
     << "Vendor Product code: " << ddi.GetVendorProductCode() << std::endl
     << "UID: " << ddi.GetUID() << std::endl
     << "Interface GUID: " << ddi.GetInterfaceGUID() << std::endl;
  return os;
}
