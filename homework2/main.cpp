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

class Item {
  // Думал о protected, но на лекциях сказали, что аксессорные методы круче
 private:
  std::string id_;
  std::string name_;

 public:
  Item(const std::string& id, const std::string& name)
      : id_(id), name_(name) {};

  virtual std::string Describe() const = 0;

  virtual std::string Details(int days = 0, double tariff = 0.0) const {
    return Describe();
  }

  const std::string& Getid() const { return id_; }
  const std::string& Getname() const { return name_; }

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
 private:
  int noviceScore_;

 protected:
  // Функция для реализации творческой части задания
  virtual std::string ExtraDetails(int days, double tariff) const { return ""; }

 public:
  Transport(const std::string& id, const std::string& name, int score)
      : Item(id, name), noviceScore_(score) {
    if (score < 1 || score > 10) {
      throw InvalidArgumentError("Invalid novice score");
    }
  }

  std::string Describe() const override { return Getid() + " " + Getname(); }

  // Функцию добавил для реализации творческой части задания
  virtual std::string TransportType() const = 0;

  // Творческая часть. Функция для подробного вывода информации о ТС.
  std::string Details(int days = 0, double tariff = 0.0) const override {
    std::string result =
        "Тип транспорта: " + TransportType() + "\n" + "ID: " + Getid() + "\t" +
        "Имя: " + Getname() + "\n" +
        "Простота для новичка: " + std::to_string(noviceScore_) + "\n" +
        ExtraDetails(days, tariff);
    return result;
  }

  bool IncludeInNoviceList() const override { return noviceScore_ >= 6; }

  bool RequiresInspection() const override { return true; }
};

class Bike : public Transport {
 public:
  Bike(const std::string& id, const std::string& name, int score)
      : Transport(id, name, score) {}
  std::string TransportType() const override { return "Велосипед"; }
};

class EnergyConsumer {
 private:
  double energy_;

 public:
  explicit EnergyConsumer(double energy) : energy_(energy) {
    if (energy < 0.0) {
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
    if (tariff < 0) {
      throw InvalidArgumentError("Invalid tariff value");
    }
    return EnergyForDays(days) * tariff;
  };

  virtual ~EnergyConsumer() = default;
};

class Scooter : public Transport, public EnergyConsumer {
 protected:
  // Функция для реализации творческого задания
  std::string ExtraDetails(int days, double tariff) const override {
    if (days == 0) {
      return std::format("Суточное потребление: {:.1f} кВт·ч\n", DailyEnergy());
    }
    return std::format(
        "Суточное потребление: {:.1f} кВт·ч\n"
        "Потребление за {} дней: {:.1f} кВт·ч\n"
        "Стоимость энергии: {:.1f} руб.\n",
        DailyEnergy(),
        days,
        EnergyForDays(days),
        EnergyCost(days, tariff)
    );      
  }

 public:
  Scooter(const std::string& id, const std::string& name, double energy,
          int score)
      : Transport(id, name, score), EnergyConsumer(energy) {}

  double DailyEnergy() const override { return GetEnergy(); }

  std::string TransportType() const override { return "Скутер"; }
};

int main() {
  Scooter scooter("S-1", "ModelX", 2.5, 8);

  const Item& item2 = scooter;

  std::cout << item2 << '\n';
  std::cout << item2.DailyEnergy() << '\n';
  std::cout << item2.IncludeInNoviceList() << '\n';

  std::cout << scooter.Details(10, 10.0) << '\n';
}