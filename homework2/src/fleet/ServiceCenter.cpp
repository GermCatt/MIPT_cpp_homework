#include "fleet/ServiceCenter.hpp"

#include "common/Errors.hpp"

#include <algorithm>
#include <cctype>
#include <string>

void ServiceCenter::Inspect(const Item& item) const {
  std::string name_lowercase = item.GetName();
  std::transform(name_lowercase.begin(), name_lowercase.end(),
                 name_lowercase.begin(),
                 [](unsigned char c) { return std::tolower(c); });
  if (name_lowercase.find("broken") != std::string::npos) {
    throw InspectionRejectedError("Inspection failed: transport is broken");
  }
}
