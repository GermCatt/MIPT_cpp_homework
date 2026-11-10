#pragma once

#include "fleet/Registry.hpp"
#include "fleet/ServiceCenter.hpp"

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

// Класс управления всем парком. Бизнес-логика здесь
class Fleet {
 private:
  Registry registry_;
  ServiceCenter serviceCenter_;
  std::weak_ptr<Item> focus_;
  bool focusWasSet_ = false;

 public:
  void Add(std::unique_ptr<Item> item);

  std::size_t Size() const;

  long double DailyEnergy() const;

  std::vector<std::weak_ptr<const Item>> NoviceList() const;

  std::vector<std::weak_ptr<const Item>> List() const;

  void Focus(const std::string& id);

  std::weak_ptr<const Item> GetFocus() const;

  bool FocusWasSet() const;

  void Remove(const std::string& id);
};
