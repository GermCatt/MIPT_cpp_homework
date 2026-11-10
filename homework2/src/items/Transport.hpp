#pragma once

#include <string>

#include "common/BoundedValue.hpp"
#include "items/EnergyConsumer.hpp"
#include "items/Inventory.hpp"

class Transport : public Item {
 private:
  BoundedValue<int> noviceScore_;

 protected:
  // Функция для реализации творческой части задания
  virtual std::string ExtraDetails() const;

 public:
  Transport(const std::string& id, const std::string& name, int score);

  const std::string& GetId() const override;
  const std::string& GetName() const override;

  // Переопределение наше, а строку собирает родитель. Тот самый костыль из
  // Item.
  std::string Describe() const override;

  // Творческая часть. Функция для подробного вывода информации о ТС.
  std::string Details() const override;

  // Функция для включения объекта в список транспорта для новичков
  bool IncludeInNoviceList() const override;

  // Отвечает на вопрос нужен ли техосмотр
  bool RequiresInspection() const override;
};

// Общий класс для всего электротранспорта
class ElectricTransport : public Transport, public EnergyConsumer {
 public:
  ElectricTransport(const std::string& id, const std::string& name,
                    double energy, int score);

  double DailyEnergy() const override;
};

class Bike : public Transport {
 public:
  Bike(const std::string& id, const std::string& name, int score);
  // Творческая часть задания. Функция описывает какой это тип транспорта
  std::string TypeName() const override;
};

// Электросамокат
class Scooter : public ElectricTransport {
 public:
  Scooter(const std::string& id, const std::string& name, double energy,
          int score);
  // Творческая часть задания. Функция описывает какой это тип транспорта
  std::string TypeName() const override;
};

// Электровелик
class ElectricBike : public ElectricTransport {
 public:
  ElectricBike(const std::string& id, const std::string& name, double energy,
               int score);

  std::string TypeName() const override;
};
