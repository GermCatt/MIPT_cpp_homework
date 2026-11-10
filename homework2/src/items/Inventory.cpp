#include "items/Inventory.hpp"

#include <ostream>

Item::Item(const std::string& id, const std::string& name)
    : id_(id), name_(name) {}

bool Item::IncludeInNoviceList() const { return false; }

bool Item::RequiresInspection() const { return false; }

// Да, это тела чисто виртуальных функций. Нет, = 0 выше не опечатка.
std::string Item::Describe() const { return GetId() + " " + GetName(); }

const std::string& Item::GetId() const { return id_; }
const std::string& Item::GetName() const { return name_; }

std::ostream& operator<<(std::ostream& os, const Item& item) {
  /*
  Тут я перегрузил оператор вывода в классе-родителе.
  У каждого ребёнка свой Describe, они его переопределяют
  Таким образом удалось избежать многократного определения этого оператора для
  разных детей
  */
  os << item.Describe();
  return os;
}
