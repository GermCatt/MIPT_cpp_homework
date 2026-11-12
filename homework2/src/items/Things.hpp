#pragma once

#include <string>

#include "common/Errors.hpp"
#include "items/EnergyConsumer.hpp"
#include "items/Inventory.hpp"

// Класс для фсякой фигни типа шлемов, налокотников и док станций
class Equipment : public Item {
 protected:
  virtual std::string ExtraDetails() const;

 public:
  Equipment(const std::string& id, const std::string& name);
  const std::string& GetId() const override;
  const std::string& GetName() const override;

  // Переопределение наше, а строку собирает родитель. Тот самый костыль из
  // Item.
  std::string Describe() const override;

  std::string Details() const override;
};

class Helmet : public Equipment {
 private:
  struct Size {
   private:
    // Творческая часть задания. Я добавил шлему характеристику размера
    std::string size_;

   public:
    Size(const std::string& size) {
      if (size != "S" && size != "M" && size != "L" && size != "XL") {
        throw InvalidArgumentError("Размер шлема должен быть S, M, L или XL.");
      }
      size_ = size;
    }
    const std::string& get() const { return size_; }
  };
  Size size_;

 protected:
  std::string ExtraDetails() const override;

 public:
  Helmet(const std::string& id, const std::string& name,
         const std::string size);

  std::string TypeName() const override;
};

// Творческая часть задания. Я добавил налокотники
class ElbowPads : public Equipment {
 protected:
  //
  std::string ExtraDetails() const override;

 public:
  ElbowPads(const std::string& id, const std::string& name);

  std::string TypeName() const override;
};

class DocStation : public Equipment, public EnergyConsumer {
 private:
  std::string district_;

 protected:
  std::string ExtraDetails() const override;

 public:
  DocStation(const std::string& id, const std::string& name, double energy,
             const std::string& district);
  double DailyEnergy() const override;
  std::string TypeName() const override;
};
