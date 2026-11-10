#pragma once

#include "items/Inventory.hpp"

// Класс сервисного центра. Отвечает за проверку транспорта на сломанность
class ServiceCenter {
 public:
  void Inspect(const Item& item) const;
};
