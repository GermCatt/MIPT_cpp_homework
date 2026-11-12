#include "fleet/Registry.hpp"

#include <algorithm>
#include <utility>

#include "common/Errors.hpp"

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
    throw InvalidArgumentError("Не передан объект для добавления.");
  }
  if (item->GetId().empty()) {
    throw InvalidArgumentError("Инвентарный номер не может быть пустым.");
  }
  if (Contains(item->GetId())) {
    throw InvalidArgumentError("Объект с ID \"" + item->GetId() +
                               "\" уже существует.");
  }

  items_.push_back(std::move(item));
}

std::shared_ptr<Item> Registry::Find(const std::string& id) const {
  for (const auto& i : items_) {
    if (i->GetId() == id) {
      return i;
    }
  }
  throw InvalidArgumentError("Объект с ID \"" + id + "\" не найден.");
}

void Registry::Remove(const std::string& id) {
  auto it = std::find(items_.begin(), items_.end(), Find(id));
  items_.erase(it);
}

size_t Registry::Size() const { return items_.size(); }

const std::vector<std::shared_ptr<Item>>& Registry::GetItems() const {
  return items_;
}
