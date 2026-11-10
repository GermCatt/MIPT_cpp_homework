#pragma once

#include "items/Inventory.hpp"

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

/*
В целом-то сама по себе идея с Registry мне нравится. Просто я не вижу смысла
делать его шаблонным Напомню, шаблонный класс у меня уже есть - BoundedValue. Он
проверяет корректность параметров функций и полей классов
*/
class Registry {
 private:
  std::vector<std::shared_ptr<Item>> items_;

 public:
  bool Contains(const std::string& id) const;

  void Add(std::shared_ptr<Item> item);

  std::shared_ptr<Item> Find(const std::string& id) const;

  void Remove(const std::string& id);

  size_t Size() const;

  const std::vector<std::shared_ptr<Item>>& GetItems() const;
};
