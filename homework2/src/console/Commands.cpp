#include "console/Commands.hpp"

#include "items/EnergyConsumer.hpp"

#include <format>

void Commands::PrintItems(const std::vector<std::weak_ptr<const Item>>& items) const {
  for (const auto& weakItem : items) {
    if (auto item = weakItem.lock()) {
      std::cout << *item << '\n';
    }
  }
}

void Commands::Remove() {
  if (!fleet_.FocusWasSet()) {
    std::cout << "Витрина ещё не установлена\n";
    return;
  }

  auto item = fleet_.GetFocus().lock();
  if (!item) {
    std::cout << "removed\n";
    return;
  }

  // Берём ID именно живого объекта на витрине. Если старый удалили и создали
  // новый с тем же ID, weak_ptr на него сам собой не перепрыгнет.
  const std::string id = item->GetId();
  item.reset();
  Remove(id);
}

void Commands::Remove(const std::string& id) {
  try {
    fleet_.Remove(id);
  } catch (const EmptyRegistryError&) {
    std::cout << "Парк пуст\n";
  } catch (const InvalidArgumentError&) {
    std::cout << "Объект с ID " << id << " не найден\n";
  } catch (const FleetError& e) {
    std::cout << "error: " << e.what() << '\n';
  }
}

void Commands::List() const {
  try {
    PrintItems(fleet_.List());
  } catch (const EmptyRegistryError&) {
    std::cout << "Парк пуст\n";
  } catch (const FleetError& e) {
    std::cout << "error: " << e.what() << '\n';
  }
}

void Commands::NoviceList() const {
  try {
    PrintItems(fleet_.NoviceList());
  } catch (const EmptyRegistryError&) {
    std::cout << "Парк пуст\n";
  } catch (const NoNoviceTransport&) {
    std::cout << "Нет транспорта, подходящего для новичков\n";
  } catch (const FleetError& e) {
    std::cout << "error: " << e.what() << '\n';
  }
}

void Commands::Focus(const std::string& id) {
  try {
    fleet_.Focus(id);
  } catch (const EmptyRegistryError&) {
    std::cout << "Парк пуст\n";
  } catch (const InvalidArgumentError&) {
    std::cout << "Объект с ID " << id << " не найден\n";
  } catch (const FleetError& e) {
    std::cout << "error: " << e.what() << '\n';
  }
}

void Commands::ShowFocus() const {
  if (!fleet_.FocusWasSet()) {
    std::cout << "Витрина ещё не установлена\n";
    return;
  }

  if (auto item = fleet_.GetFocus().lock()) {
    std::cout << *item << '\n';
  } else {
    std::cout << "removed\n";
  }
}

void Commands::Energy() const {
  try {
    std::cout << std::format("{:.1f} кВт·ч\n", fleet_.DailyEnergy());
  } catch (const NoEnergyConsumers&) {
    std::cout << "В парке нет потребителей энергии\n0.0 кВт·ч\n";
  } catch (const EmptyRegistryError&) {
    std::cout << "Парк пуст\n0.0 кВт·ч\n";
  } catch (const FleetError& e) {
    std::cout << "error: " << e.what() << '\n';
  }
}

void Commands::Count() const { std::cout << fleet_.Size() << '\n'; }

void Commands::Details() const {
  if (!fleet_.FocusWasSet()) {
    std::cout << "Витрина ещё не установлена\n";
    return;
  }

  if (auto item = fleet_.GetFocus().lock()) {
    std::cout << item->Details() << '\n';
    if (const auto* consumer =
            dynamic_cast<const EnergyConsumer*>(item.get())) {
      std::cout << std::format("Суточное потребление: {:.1f} кВт·ч\n",
                               consumer->DailyEnergy());
    }
  } else {
    std::cout << "removed\n";
  }
}

void Commands::Details(int days, double tariff) const {
  if (!fleet_.FocusWasSet()) {
    std::cout << "Витрина ещё не установлена\n";
    return;
  }

  if (auto item = fleet_.GetFocus().lock()) {
    if (auto consumer = dynamic_cast<const EnergyConsumer*>(item.get())) {
      try {
        const auto energyDetails = consumer->EnergyDetails(days, tariff);
        std::cout << item->Details() << '\n' << energyDetails;
      } catch (const FleetError& e) {
        std::cout << "error: " << e.what() << '\n';
      }
    } else {
      throw InvalidArgumentError("Object does not consume energy");
    }
  } else {
    std::cout << "removed\n";
  }
}
