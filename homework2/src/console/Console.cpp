#include "console/Console.hpp"

#include <iostream>
#include <string_view>
#include <utility>

#include "common/Errors.hpp"
#include "console/ConsoleOutput.hpp"
#include "items/Things.hpp"
#include "items/Transport.hpp"

// Внутренние помощники разбора команд используются только в этом файле.
namespace {
enum class Command {
  Add,
  Remove,
  List,
  Novice,
  Energy,
  Count,
  Focus,
  ShowFocus,
  Details,
  Quit
};

Command ParseCommand(std::string_view name) {
  constexpr std::pair<std::string_view, Command> commands[] = {
      {"add", Command::Add},         {"remove", Command::Remove},
      {"list", Command::List},       {"novice", Command::Novice},
      {"energy", Command::Energy},   {"count", Command::Count},
      {"focus", Command::Focus},     {"show_focus", Command::ShowFocus},
      {"details", Command::Details}, {"quit", Command::Quit},
  };

  for (const auto& [text, command] : commands) {
    if (name == text) {
      return command;
    }
  }
  throw InvalidArgumentError("Неизвестная команда.");
}

template <typename T>
T ReadNumber(std::istringstream& input) {
  std::string token;
  if (!(input >> token)) {
    throw InvalidArgumentError("Неверный формат команды.");
  }

  // Читаем число целиком: один аргумент вроде 30.5 нельзя разбить на 30 и .5.
  std::istringstream number(token);
  T value;
  if (!(number >> value) || !number.eof()) {
    throw InvalidArgumentError("Неверный формат команды.");
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
    ConsoleOutput::Error(e.what());
    return true;
  }
}

bool Console::ExecuteCommand(const std::string& line) {
  std::istringstream iss(line);

  std::string commandName;
  if (!(iss >> commandName)) {
    return true;
  }

  auto noExtraArgs = [&]() {
    std::string extra;
    return !(iss >> extra);
  };

  switch (ParseCommand(commandName)) {
    case Command::Add:
      ParseAdd(iss);
      break;

    case Command::Remove: {
      std::string id;

      if (iss >> id) {
        if (!noExtraArgs()) {
          throw InvalidArgumentError("Неверный формат команды.");
        }
        commands_.Remove(id);
      } else {
        commands_.Remove();
      }
      break;
    }

    case Command::List:
      if (!noExtraArgs()) {
        throw InvalidArgumentError("Неверный формат команды.");
      }

      commands_.List();
      break;

    case Command::Novice:
      if (!noExtraArgs()) {
        throw InvalidArgumentError("Неверный формат команды.");
      }

      commands_.NoviceList();
      break;

    case Command::Energy:
      if (!noExtraArgs()) {
        throw InvalidArgumentError("Неверный формат команды.");
      }

      commands_.Energy();
      break;

    case Command::Count:
      if (!noExtraArgs()) {
        throw InvalidArgumentError("Неверный формат команды.");
      }

      commands_.Count();
      break;

    case Command::Focus: {
      std::string id;

      if (!(iss >> id) || !noExtraArgs()) {
        throw InvalidArgumentError("Неверный формат команды.");
      }

      commands_.Focus(id);
      break;
    }

    case Command::ShowFocus:
      if (!noExtraArgs()) {
        throw InvalidArgumentError("Неверный формат команды.");
      }

      commands_.ShowFocus();
      break;

    case Command::Details:
      ParseDetails(iss);
      break;

    case Command::Quit:
      if (!noExtraArgs()) {
        throw InvalidArgumentError("Неверный формат команды.");
      }

      return false;
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
    throw InvalidArgumentError("Неверный формат команды.");
  }

  if (type == "scooter") {
    const double energy = ReadNumber<double>(iss);
    const int score = ReadNumber<int>(iss);

    if (!noExtraArgs()) {
      throw InvalidArgumentError("Неверный формат команды.");
    }

    commands_.Add<Scooter>(id, name, energy, score);
  }

  else if (type == "bike") {
    const int score = ReadNumber<int>(iss);

    if (!noExtraArgs()) {
      throw InvalidArgumentError("Неверный формат команды.");
    }

    commands_.Add<Bike>(id, name, score);
  }

  else if (type == "ebike") {
    const double energy = ReadNumber<double>(iss);
    const int score = ReadNumber<int>(iss);

    if (!noExtraArgs()) {
      throw InvalidArgumentError("Неверный формат команды.");
    }

    commands_.Add<ElectricBike>(id, name, energy, score);
  }

  else if (type == "helmet") {
    std::string size;

    if (!(iss >> size) || !noExtraArgs()) {
      throw InvalidArgumentError("Неверный формат команды.");
    }

    commands_.Add<Helmet>(id, name, size);
  }

  else if (type == "dock") {
    const double energy = ReadNumber<double>(iss);
    std::string district;

    if (!(iss >> district) || !noExtraArgs()) {
      throw InvalidArgumentError("Неверный формат команды.");
    }

    commands_.Add<DocStation>(id, name, energy, district);
  }

  else if (type == "elbowpads") {
    if (!noExtraArgs()) {
      throw InvalidArgumentError("Неверный формат команды.");
    }

    commands_.Add<ElbowPads>(id, name);
  }

  else {
    throw InvalidArgumentError("Неизвестный тип объекта.");
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
    throw InvalidArgumentError("Неверный формат команды.");
  }
  commands_.Details(days, tariff);
}
