#include "console/Commands.hpp"

#include <format>

#include "console/ConsoleOutput.hpp"
#include "items/EnergyConsumer.hpp"

void Commands::PrintItems(
    const std::vector<std::weak_ptr<const Item>>& items) const {
  for (const auto& weakItem : items) {
    if (auto item = weakItem.lock()) {
      std::cout << *item << '\n';
    }
  }
}

void Commands::Remove() {
  if (!fleet_.FocusWasSet()) {
    ConsoleOutput::Message("Витрина ещё не установлена.");
    return;
  }

  auto item = fleet_.GetFocus().lock();
  if (!item) {
    ConsoleOutput::Removed();
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
    ConsoleOutput::Message("Объект удалён");
  } catch (const FleetError& e) {
    ConsoleOutput::Error(e.what());
  }
}

void Commands::List() const {
  try {
    PrintItems(fleet_.List());
  } catch (const EmptyRegistryError& e) {
    ConsoleOutput::Message(e.what());
  } catch (const FleetError& e) {
    ConsoleOutput::Error(e.what());
  }
}

void Commands::NoviceList() const {
  try {
    PrintItems(fleet_.NoviceList());
  } catch (const EmptyRegistryError& e) {
    ConsoleOutput::Message(e.what());
  } catch (const NoNoviceTransport& e) {
    ConsoleOutput::Message(e.what());
  } catch (const FleetError& e) {
    ConsoleOutput::Error(e.what());
  }
}

void Commands::Focus(const std::string& id) {
  try {
    fleet_.Focus(id);
  } catch (const FleetError& e) {
    ConsoleOutput::Error(e.what());
  }
}

void Commands::ShowFocus() const {
  if (!fleet_.FocusWasSet()) {
    ConsoleOutput::Message("Витрина ещё не установлена.");
    return;
  }

  if (auto item = fleet_.GetFocus().lock()) {
    std::cout << *item << '\n';
  } else {
    ConsoleOutput::Removed();
  }
}

void Commands::Energy() const {
  try {
    std::cout << std::format("{:.1f} кВт·ч\n", fleet_.DailyEnergy());
  } catch (const NoEnergyConsumers& e) {
    ConsoleOutput::Message(e.what());
    std::cout << "0.0 кВт·ч\n";
  } catch (const EmptyRegistryError& e) {
    ConsoleOutput::Message(e.what());
    std::cout << "0.0 кВт·ч\n";
  } catch (const FleetError& e) {
    ConsoleOutput::Error(e.what());
  }
}

void Commands::Count() const { std::cout << fleet_.Size() << '\n'; }

void Commands::Details() const {
  if (!fleet_.FocusWasSet()) {
    ConsoleOutput::Message("Витрина ещё не установлена.");
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
    ConsoleOutput::Removed();
  }
}

void Commands::Details(int days, double tariff) const {
  if (!fleet_.FocusWasSet()) {
    ConsoleOutput::Message("Витрина ещё не установлена.");
    return;
  }

  if (auto item = fleet_.GetFocus().lock()) {
    if (auto consumer = dynamic_cast<const EnergyConsumer*>(item.get())) {
      try {
        const auto energyDetails = consumer->EnergyDetails(days, tariff);
        std::cout << item->Details() << '\n' << energyDetails;
      } catch (const FleetError& e) {
        ConsoleOutput::Error(e.what());
      }
    } else {
      throw InvalidArgumentError("Выбранный объект не потребляет энергию.");
    }
  } else {
    ConsoleOutput::Removed();
  }
}
