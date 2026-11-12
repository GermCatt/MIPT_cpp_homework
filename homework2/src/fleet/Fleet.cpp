#include "fleet/Fleet.hpp"

#include <algorithm>
#include <cmath>
#include <utility>

#include "common/Errors.hpp"
#include "items/EnergyConsumer.hpp"

void Fleet::Add(std::unique_ptr<Item> item) {
  if (!item) {
    throw InvalidArgumentError("Не передан объект для добавления.");
  }

  if (item->RequiresInspection()) {
    serviceCenter_.Inspect(*item);
  };

  registry_.Add(std::move(item));
}

std::size_t Fleet::Size() const { return registry_.Size(); }

long double Fleet::DailyEnergy() const {
  if (!registry_.Size()) {
    throw EmptyRegistryError("Парк пуст.");
  }

  std::size_t consumersCount = 0;
  long double result = 0.0;

  for (const auto& item : registry_.GetItems()) {
    if (const auto* consumer =
            dynamic_cast<const EnergyConsumer*>(item.get())) {
      result += consumer->DailyEnergy();
      ++consumersCount;
    }
  }

  if (consumersCount == 0) {
    throw NoEnergyConsumers("В парке нет потребителей энергии.");
  }
  if (!std::isfinite(result)) {
    throw EnergyCalculationError("Переполнение при расчёте энергии парка.");
  }

  return result;
}

std::vector<std::weak_ptr<const Item>> Fleet::NoviceList() const {
  if (!registry_.Size()) {
    throw EmptyRegistryError("Парк пуст.");
  }

  std::vector<std::weak_ptr<const Item>> result;

  for (const auto& i : registry_.GetItems()) {
    if (i->IncludeInNoviceList()) {
      result.push_back(i);
    }
  }
  if (result.empty()) {
    throw NoNoviceTransport("Нет транспорта, подходящего для новичков.");
  }

  std::sort(result.begin(), result.end(), [](const auto& lhs, const auto& rhs) {
    auto lhs_item = lhs.lock();
    auto rhs_item = rhs.lock();

    if (lhs_item->TypeName() != rhs_item->TypeName()) {
      return lhs_item->TypeName() < rhs_item->TypeName();
    }

    return lhs_item->GetId() < rhs_item->GetId();
  });
  return result;
}

std::vector<std::weak_ptr<const Item>> Fleet::List() const {
  if (!registry_.Size()) {
    throw EmptyRegistryError("Парк пуст.");
  }

  std::vector<std::weak_ptr<const Item>> result;

  for (const auto& i : registry_.GetItems()) {
    result.push_back(i);
  }

  std::sort(result.begin(), result.end(), [](const auto& lhs, const auto& rhs) {
    auto lhs_item = lhs.lock();
    auto rhs_item = rhs.lock();

    return *lhs_item < *rhs_item;
  });
  return result;
}

void Fleet::Focus(const std::string& id) {
  focus_ = registry_.Find(id);
  focusWasSet_ = true;
}

std::weak_ptr<const Item> Fleet::GetFocus() const { return focus_; }

bool Fleet::FocusWasSet() const { return focusWasSet_; }

void Fleet::Remove(const std::string& id) { registry_.Remove(id); }
