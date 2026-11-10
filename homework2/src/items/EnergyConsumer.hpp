#pragma once

#include "common/BoundedValue.hpp"

#include <string>

// Общий класс-родитель для всех потребителей энергии
class EnergyConsumer {
 private:
  // На сто лет вперёд не гадаем, а тариф дороже 9999 рублей уже пугает.
  static constexpr int maxDays_ = 365;
  static constexpr double maxTariff_ = 9999.0;
  BoundedValue<double> energy_;

 public:
  explicit EnergyConsumer(double energy);

  double GetEnergy() const;

  virtual double DailyEnergy() const = 0;

  // Функцию добавил для реализации творческой части задания
  double EnergyForDays(const BoundedValue<int>& days) const;

  // Функцию добавил для реализации творческой части задания
  double EnergyCost(const BoundedValue<int>& days,
                    const BoundedValue<double>& tariff) const;

  std::string EnergyDetails(int days, double tariff) const;

  virtual ~EnergyConsumer() = default;
};
