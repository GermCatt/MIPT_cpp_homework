#include <cmath>
#include <format>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

class FleetError : public std::runtime_error {
 public:
  explicit FleetError(const std::string& message)
      : std::runtime_error(message) {}
};

class InvalidArgumentError : public FleetError {
 public:
  explicit InvalidArgumentError(const std::string& message)
      : FleetError(message) {}
};

struct CalculationSettings {
  int days = 10;
  double tariff = 10.0;
};

const std::unique_ptr<CalculationSettings> calculationSettings =
    std::make_unique<CalculationSettings>();

class Item {
 public:
  virtual std::string Describe() const = 0;

  virtual std::string Details() const = 0;

  virtual const std::string& GetId() const = 0;
  virtual const std::string& GetName() const = 0;

  virtual double DailyEnergy() const { return 0.0; }

  // Функция для составления списка трансопрта, пригодного для новичков. Чтобы
  // при вызове функцииц печати транспорта пригодного для новичков не делать
  // проверку типов, для любого объекта базовое значение функции - false. В
  // классе транспорта эта функция будет переопределена
  virtual bool IncludeInNoviceList() const { return false; }

  // Функция для техосмотра. Там, где он нужен, она переопределна на true. Таким
  // образом, при добавлении в парк мы не проверяем типы, а просто вызываем
  // функцию, которая есть у любого наследника item
  virtual bool RequiresInspection() const { return false; }

  virtual ~Item() = default;
};

std::ostream& operator<<(std::ostream& os, const Item& item) {
  /*
  Тут я перегрузил оператор вывода в классе-родителе.
  У каждого ребёнка свой Describe, они его переопределяют
  Таким образом удалось избежать многократного определения этого оператора для
  разных детей
  */
  os << item.Describe();
  return os;
}

class Transport : public Item {
  // Думал о protected, но на лекциях сказали, что аксессорные методы круче
 private:
  std::string id_;
  std::string name_;
  int noviceScore_;

 protected:
  // Функция для реализации творческой части задания
  virtual std::string ExtraDetails() const { return ""; }

 public:
  Transport(const std::string& id, const std::string& name, int score)
      : id_(id), name_(name), noviceScore_(score) {
    if (score < 1 || score > 10) {
      throw InvalidArgumentError("Invalid novice score");
    }
  }

  const std::string& GetId() const override { return id_; }
  const std::string& GetName() const override { return name_; }

  std::string Describe() const override { return GetId() + " " + GetName(); }

  // Функцию добавил для реализации творческой части задания
  virtual std::string TransportType() const = 0;

  // Творческая часть. Функция для подробного вывода информации о ТС.
  std::string Details() const override {
    std::string result =
        "Тип транспорта: " + TransportType() + "\n" + "ID: " + GetId() + "\t" +
        "Имя: " + GetName() + "\n" +
        "Простота для новичка: " + std::to_string(noviceScore_) + "\n" +
        ExtraDetails();
    return result;
  }

  bool IncludeInNoviceList() const override { return noviceScore_ >= 6; }

  bool RequiresInspection() const override { return true; }
};

class EnergyConsumer {
 private:
  double energy_;

 public:
  explicit EnergyConsumer(double energy) : energy_(energy) {
    if (!std::isfinite(energy) || energy < 0.0) {
      throw InvalidArgumentError("Invalid energy consume");
    }
  };

  double GetEnergy() const { return energy_; }

  virtual double DailyEnergy() const = 0;

  // Функцию добавил для реализации творческой части задания
  double EnergyForDays(int days) const {
    if (days < 0) {
      throw InvalidArgumentError("Invalid days value");
    }
    return DailyEnergy() * days;
  };

  // Функцию добавил для реализации творческой части задания
  double EnergyCost(int days, double tariff) const {
    if (!std::isfinite(tariff) || tariff < 0) {
      throw InvalidArgumentError("Invalid tariff value");
    }
    return EnergyForDays(days) * tariff;
  };

  virtual ~EnergyConsumer() = default;
};

class ElectricTransport : public Transport, public EnergyConsumer {
 protected:
  // Функция для реализации творческого задания
  std::string ExtraDetails() const override {
    const int days = calculationSettings->days;
    const double tariff = calculationSettings->tariff;
    return std::format(
        "Суточное потребление: {:.1f} кВт·ч\n"
        "Потребление за {} дней: {:.1f} кВт·ч\n"
        "Стоимость энергии: {:.1f} руб.\n",
        DailyEnergy(), days, EnergyForDays(days), EnergyCost(days, tariff));
  }

 public:
  ElectricTransport(const std::string& id, const std::string& name,
                    double energy, int score)
      : Transport(id, name, score), EnergyConsumer(energy) {}

  double DailyEnergy() const override { return GetEnergy(); }
};

class Bike : public Transport {
 public:
  Bike(const std::string& id, const std::string& name, int score)
      : Transport(id, name, score) {}
  std::string TransportType() const override { return "Велосипед"; }
};

class Scooter : public ElectricTransport {
 public:
  Scooter(const std::string& id, const std::string& name, double energy,
          int score)
      : ElectricTransport(id, name, energy, score) {}

  std::string TransportType() const override { return "Электросамокат"; }
};

class ElectricBike : public ElectricTransport {
 public:
  ElectricBike(const std::string& id, const std::string& name, double energy,
               int score)
      : ElectricTransport(id, name, energy, score) {}

  std::string TransportType() const override { return "Электровелосипед"; }
};

class Equipment : public Item {
 private:
  std::string id_;
  std::string name_;

 protected:
  virtual std::string ExtraDetails() const { return ""; }

 public:
  Equipment(const std::string& id, const std::string& name)
      : id_(id), name_(name) {};
  const std::string& GetId() const override { return id_; }
  const std::string& GetName() const override { return name_; }

  std::string Describe() const override { return GetId() + " " + GetName(); }

  virtual std::string EquipmentType() const = 0;

  std::string Details() const override {
    std::string result = "Тип оборудования: " + EquipmentType() + "\n" +
                         "ID: " + GetId() + "\t" + "Имя: " + GetName() + "\n" +
                         ExtraDetails();
    return result;
  }
};

class Helmet : public Equipment {
 private:
  struct Size {
   private:
    std::string size_;

   public:
    Size(const std::string& size) {
      if (size != "S" && size != "M" && size != "L" && size != "XL") {
        throw InvalidArgumentError("Invalid size");
      }
      size_ = size;
    }
    const std::string& get() const { return size_; }
  };
  Size size_;

 protected:
  std::string ExtraDetails() const override {
    return "Необходим для новичков!\nРазмер: " + size_.get() + "\n";
  }

 public:
  Helmet(const std::string& id, const std::string& name, const std::string size)
      : Equipment(id, name), size_(size) {};

  std::string EquipmentType() const override { return "Шлем"; }
};

class ElbowPads : public Equipment {
 protected:
  std::string ExtraDetails() const override {
    return "Необходимы для новичков!\n";
  }

 public:
  ElbowPads(const std::string& id, const std::string& name)
      : Equipment(id, name) {};

  std::string EquipmentType() const override { return "Налокотники"; }
};

class DocStation : public Equipment, public EnergyConsumer {
 private:
  std::string district_;

 protected:
  std::string ExtraDetails() const override {
    const int days = calculationSettings->days;
    const double tariff = calculationSettings->tariff;
    return std::format(
        "Станция относится к району: {}\n"
        "Суточное потребление: {:.1f} кВт·ч\n"
        "Потребление за {} дней: {:.1f} кВт·ч\n"
        "Стоимость энергии: {:.1f} руб.\n",
        district_, DailyEnergy(), days, EnergyForDays(days),
        EnergyCost(days, tariff));
  }

 public:
  DocStation(const std::string& id, const std::string& name, double energy,
             const std::string& district)
      : Equipment(id, name), EnergyConsumer(energy), district_(district) {};
  double DailyEnergy() const override { return GetEnergy(); }
  std::string EquipmentType() const override { return "Док-станция"; }
};