#include "items/Transport.hpp"

std::string Transport::ExtraDetails() const { return ""; }

Transport::Transport(const std::string& id, const std::string& name, int score)
    : Item(id, name), noviceScore_(score, 1, 10, "Invalid novice score") {}

const std::string& Transport::GetId() const { return Item::GetId(); }

const std::string& Transport::GetName() const { return Item::GetName(); }

std::string Transport::Describe() const { return Item::Describe(); }

std::string Transport::Details() const {
  std::string result =
      "Тип транспорта: " + TypeName() + "\n" + "ID: " + GetId() + "\t" +
      "Имя: " + GetName() + "\n" +
      "Простота для новичка: " + std::to_string(noviceScore_.Get()) + "\n" +
      ExtraDetails();
  return result;
}

bool Transport::IncludeInNoviceList() const { return noviceScore_ >= 6; }

bool Transport::RequiresInspection() const { return true; }

ElectricTransport::ElectricTransport(const std::string& id,
                                     const std::string& name, double energy,
                                     int score)
    : Transport(id, name, score), EnergyConsumer(energy) {}

double ElectricTransport::DailyEnergy() const { return GetEnergy(); }

Bike::Bike(const std::string& id, const std::string& name, int score)
    : Transport(id, name, score) {}

std::string Bike::TypeName() const { return "Велосипед"; }

Scooter::Scooter(const std::string& id, const std::string& name, double energy,
                 int score)
    : ElectricTransport(id, name, energy, score) {}

std::string Scooter::TypeName() const { return "Электросамокат"; }

ElectricBike::ElectricBike(const std::string& id, const std::string& name,
                           double energy, int score)
    : ElectricTransport(id, name, energy, score) {}

std::string ElectricBike::TypeName() const { return "Электровелосипед"; }
