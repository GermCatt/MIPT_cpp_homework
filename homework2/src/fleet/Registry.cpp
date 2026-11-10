#include "fleet/Registry.hpp"

#include "common/Errors.hpp"

#include <algorithm>
#include <utility>

bool Registry::Contains(const std::string& id) const {
  for (const auto& i : items_) {
    if (i->GetId() == id) {
      return true;
    }
  }
  return false;
}

void Registry::Add(std::shared_ptr<Item> item) {
  if (!item) {
    throw InvalidArgumentError("Null item");
  }
  if (item->GetId().empty()) {
    throw InvalidArgumentError("Empty ID");
  }
  if (Contains(item->GetId())) {
    throw InvalidArgumentError(
        "Duplicate: object with this ID already exists");
  }

  items_.push_back(std::move(item));
}

std::shared_ptr<Item> Registry::Find(const std::string& id) const {
  if (!Size()) {
    throw EmptyRegistryError("Registry is empty");
  }

  for (const auto& i : items_) {
    if (i->GetId() == id) {
      return i;
    }
  }
  throw InvalidArgumentError("Invalid Id: id not in the list");
}

void Registry::Remove(const std::string& id) {
  auto it = std::find(items_.begin(), items_.end(), Find(id));
  items_.erase(it);
}

size_t Registry::Size() const { return items_.size(); }

const std::vector<std::shared_ptr<Item>>& Registry::GetItems() const { return items_; }
