#include "items/EnergyConsumer.hpp"

#include <cmath>
#include <format>
#include <limits>

#include "common/Errors.hpp"

EnergyConsumer::EnergyConsumer(double energy)
    : energy_(energy, 0, std::numeric_limits<double>::max(),
              "Invalid energy consume") {}

double EnergyConsumer::GetEnergy() const { return energy_.Get(); }

double EnergyConsumer::EnergyForDays(const BoundedValue<int>& days) const {
  if (days < 0 || days > maxDays_) {
    throw EnergyCalculationError("Days must be between 0 and 365");
  }
  const double result = days * DailyEnergy();
  // Значение может стать бесконечным, если потребитель потребляет очень много.
  // Ограничения дней тут сами по себе не спасают.
  if (!std::isfinite(result)) {
    throw EnergyCalculationError("Energy calculation overflow");
  }
  return result;
}

double EnergyConsumer::EnergyCost(const BoundedValue<int>& days,
                                  const BoundedValue<double>& tariff) const {
  if (tariff < 0 || tariff > maxTariff_) {
    throw EnergyCalculationError("Tariff must be between 0 and 9999");
  }
  const double result = tariff * EnergyForDays(days);
  if (!std::isfinite(result)) {
    throw EnergyCalculationError("Energy cost calculation overflow");
  }
  return result;
}

std::string EnergyConsumer::EnergyDetails(int days, double tariff) const {
  try {
    BoundedValue<int> validDays(days, 0, maxDays_,
                                "Days must be between 0 and 365");
    BoundedValue<double> validTariff(tariff, 0.0, maxTariff_,
                                     "Tariff must be between 0 and 9999");

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
