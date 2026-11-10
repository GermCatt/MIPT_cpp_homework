#include "items/Things.hpp"

std::string Equipment::ExtraDetails() const { return ""; }

Equipment::Equipment(const std::string& id, const std::string& name)
    : Item(id, name) {}

const std::string& Equipment::GetId() const { return Item::GetId(); }

const std::string& Equipment::GetName() const { return Item::GetName(); }

std::string Equipment::Describe() const { return Item::Describe(); }

std::string Equipment::Details() const {
  std::string result = "Тип оборудования: " + TypeName() + "\n" +
                       "ID: " + GetId() + "\t" + "Имя: " + GetName() + "\n" +
                       ExtraDetails();
  return result;
}

std::string Helmet::ExtraDetails() const {
  return "Необходим для новичков!\nРазмер: " + size_.get() + "\n";
}

Helmet::Helmet(const std::string& id, const std::string& name, const std::string size)
    : Equipment(id, name), size_(size) {}

std::string Helmet::TypeName() const { return "Шлем"; }

std::string ElbowPads::ExtraDetails() const {
  return "Необходимы для новичков!\n";
}

ElbowPads::ElbowPads(const std::string& id, const std::string& name)
    : Equipment(id, name) {}

std::string ElbowPads::TypeName() const { return "Налокотники"; }

std::string DocStation::ExtraDetails() const {
  return "Станция относится к району: " + district_ + "\n";
}

DocStation::DocStation(const std::string& id, const std::string& name, double energy,
           const std::string& district)
    : Equipment(id, name), EnergyConsumer(energy), district_(district) {}

double DocStation::DailyEnergy() const { return GetEnergy(); }

std::string DocStation::TypeName() const { return "Док-станция"; }
