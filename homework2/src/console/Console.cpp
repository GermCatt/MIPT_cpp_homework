#include "console/Console.hpp"

#include <iostream>

#include "common/Errors.hpp"
#include "items/Things.hpp"
#include "items/Transport.hpp"

// Ограничиваем область видмости функции чтения числа.
// Она должна использоваться только здесь
namespace {
template <typename T>
T ReadNumber(std::istringstream& input) {
  std::string token;
  if (!(input >> token)) {
    throw InvalidArgumentError("invalid command syntax");
  }

  // Читаем число целиком: один аргумент вроде 30.5 нельзя разбить на 30 и .5.
  std::istringstream number(token);
  T value;
  if (!(number >> value) || !number.eof()) {
    throw InvalidArgumentError("invalid command syntax");
  }
  return value;
}
}  // namespace

void Console::Run() {
  std::string line;
  while (std::getline(std::cin, line)) {
    if (!Execute(line)) {
      break;
    }
  }
}

bool Console::Execute(const std::string& line) {
  try {
    return ExecuteCommand(line);
  } catch (const FleetError& e) {
    std::cout << "error: " << e.what() << '\n';
    return true;
  }
}

bool Console::ExecuteCommand(const std::string& line) {
  std::istringstream iss(line);

  std::string command;
  if (!(iss >> command)) {
    return true;
  }

  auto noExtraArgs = [&]() {
    std::string extra;
    return !(iss >> extra);
  };

  if (command == "add") {
    ParseAdd(iss);
  }

  else if (command == "remove") {
    std::string id;

    if (iss >> id) {
      if (!noExtraArgs()) {
        throw InvalidArgumentError("invalid command syntax");
      }
      commands_.Remove(id);
    } else {
      commands_.Remove();
    }
  }

  else if (command == "list") {
    if (!noExtraArgs()) {
      throw InvalidArgumentError("invalid command syntax");
    }

    commands_.List();
  }

  else if (command == "novice") {
    if (!noExtraArgs()) {
      throw InvalidArgumentError("invalid command syntax");
    }

    commands_.NoviceList();
  }

  else if (command == "energy") {
    if (!noExtraArgs()) {
      throw InvalidArgumentError("invalid command syntax");
    }

    commands_.Energy();
  }

  else if (command == "count") {
    if (!noExtraArgs()) {
      throw InvalidArgumentError("invalid command syntax");
    }

    commands_.Count();
  }

  else if (command == "focus") {
    std::string id;

    if (!(iss >> id) || !noExtraArgs()) {
      throw InvalidArgumentError("invalid command syntax");
    }

    commands_.Focus(id);
  }

  else if (command == "show_focus") {
    if (!noExtraArgs()) {
      throw InvalidArgumentError("invalid command syntax");
    }

    commands_.ShowFocus();
  }

  else if (command == "details") {
    ParseDetails(iss);
  }

  else if (command == "quit") {
    if (!noExtraArgs()) {
      throw InvalidArgumentError("invalid command syntax");
    }

    return false;
  }

  else {
    throw InvalidArgumentError("unknown command");
  }
  return true;
}

void Console::ParseAdd(std::istringstream& iss) {
  auto noExtraArgs = [&]() {
    std::string extra;
    return !(iss >> extra);
  };

  std::string type;
  std::string id;
  std::string name;

  if (!(iss >> type >> id >> name)) {
    throw InvalidArgumentError("invalid command syntax");
  }

  if (type == "scooter") {
    const double energy = ReadNumber<double>(iss);
    const int score = ReadNumber<int>(iss);

    if (!noExtraArgs()) {
      throw InvalidArgumentError("invalid command syntax");
    }

    commands_.Add<Scooter>(id, name, energy, score);
  }

  else if (type == "bike") {
    const int score = ReadNumber<int>(iss);

    if (!noExtraArgs()) {
      throw InvalidArgumentError("invalid command syntax");
    }

    commands_.Add<Bike>(id, name, score);
  }

  else if (type == "ebike") {
    const double energy = ReadNumber<double>(iss);
    const int score = ReadNumber<int>(iss);

    if (!noExtraArgs()) {
      throw InvalidArgumentError("invalid command syntax");
    }

    commands_.Add<ElectricBike>(id, name, energy, score);
  }

  else if (type == "helmet") {
    std::string size;

    if (!(iss >> size) || !noExtraArgs()) {
      throw InvalidArgumentError("invalid command syntax");
    }

    commands_.Add<Helmet>(id, name, size);
  }

  else if (type == "dock") {
    const double energy = ReadNumber<double>(iss);
    std::string district;

    if (!(iss >> district) || !noExtraArgs()) {
      throw InvalidArgumentError("invalid command syntax");
    }

    commands_.Add<DocStation>(id, name, energy, district);
  }

  else if (type == "elbowpads") {
    if (!noExtraArgs()) {
      throw InvalidArgumentError("invalid command syntax");
    }

    commands_.Add<ElbowPads>(id, name);
  }

  else {
    throw InvalidArgumentError("unknown item type");
  }
}

void Console::ParseDetails(std::istringstream& iss) {
  iss >> std::ws;
  if (iss.eof()) {
    commands_.Details();
    return;
  }

  const int days = ReadNumber<int>(iss);
  const double tariff = ReadNumber<double>(iss);
  std::string extra;
  if (iss >> extra) {
    throw InvalidArgumentError("invalid command syntax");
  }
  commands_.Details(days, tariff);
}
