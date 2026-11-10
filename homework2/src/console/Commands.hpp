#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "common/Errors.hpp"
#include "fleet/Fleet.hpp"


// Класс обработки комманд
class Commands {
 private:
  Fleet fleet_;

  // Просто метод вывода списка по сути
  void PrintItems(const std::vector<std::weak_ptr<const Item>>& items) const;

 public:
  // Чтобы не писать миллион одинаковых функций добавления объектов для каждого
  // типа я написал универсальный шаблонный метод добавления, который будет
  // обрабатывать все ошибки и все добавления
  template <typename T, typename... Args>
  void Add(Args&&... args) {
    try {
      fleet_.Add(std::make_unique<T>(std::forward<Args>(args)...));
    } catch (const FleetError& e) {
      std::cout << "error: " << e.what() << '\n';
    }
  }
  /*
    Говоря откровенно, я плохо понимаю, как это работает и почему здесь
    обязательно нужно сохранять тип ссылки (rvalue или lvalue), но я увидел эту
    реализацию на StackOverflow и какой-то чувак написал, что так надо.
    Я ему верю.
  */

  void Remove();
  void Remove(const std::string& id);

  void List() const;

  void NoviceList() const;

  void Focus(const std::string& id);

  void ShowFocus() const;

  void Energy() const;

  void Count() const;

  // Творческая часть. Функция выводит подробную информацию об объекте на
  // витрине. Сама подробная информация зависит от объекта и полиморфно меняется
  // каждым объектом
  void Details() const;

  void Details(int days, double tariff) const;

};
