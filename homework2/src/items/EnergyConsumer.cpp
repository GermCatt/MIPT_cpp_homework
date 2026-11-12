#include "items/EnergyConsumer.hpp"

#include <cmath>
#include <format>

#include "common/Errors.hpp"

EnergyConsumer::EnergyConsumer(double energy)
    : energy_(energy, 0.0, maxDailyEnergy_,
              "Суточное потребление должно быть от 0 до 10000 кВт·ч.") {}

double EnergyConsumer::GetEnergy() const { return energy_.Get(); }

double EnergyConsumer::EnergyForDays(const BoundedValue<int>& days) const {
  if (days < 0 || days > maxDays_) {
    throw EnergyCalculationError("Количество дней должно быть от 0 до 365.");
  }
  const double result = days * DailyEnergy();
  // Сохраняем проверку результата на случай изменения лимитов или реализации
  // виртуального DailyEnergy() у новых наследников.
  if (!std::isfinite(result)) {
    throw EnergyCalculationError("Переполнение при расчёте энергопотребления.");
  }
  return result;
}

double EnergyConsumer::EnergyCost(const BoundedValue<int>& days,
                                  const BoundedValue<double>& tariff) const {
  if (tariff < 0 || tariff > maxTariff_) {
    throw EnergyCalculationError("Тариф должен быть от 0 до 9999 руб./кВт·ч.");
  }
  const double result = tariff * EnergyForDays(days);
  if (!std::isfinite(result)) {
    throw EnergyCalculationError("Переполнение при расчёте стоимости энергии.");
  }
  return result;
}

std::string EnergyConsumer::EnergyDetails(int days, double tariff) const {
  try {
    BoundedValue<int> validDays(days, 0, maxDays_,
                                "Количество дней должно быть от 0 до 365.");
    BoundedValue<double> validTariff(
        tariff, 0.0, maxTariff_, "Тариф должен быть от 0 до 9999 руб./кВт·ч.");

    return std::format(
        "Суточное потребление: {:.1f} кВт·ч\n"
        "Потребление за {} дней: {:.1f} кВт·ч\n"
        "Стоимость энергии: {:.1f} руб.\n",
        DailyEnergy(), days, EnergyForDays(validDays),
        EnergyCost(validDays, validTariff));
  } catch (const InvalidArgumentError& e) {
    // BoundedValue не знает, что мы тут считаем деньги. Перевожу его ошибку
    // в отдельное исключение для расчётов, а печатать будет Commands.
    throw EnergyCalculationError(e.what());
  }
}
