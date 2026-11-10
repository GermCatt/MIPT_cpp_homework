#pragma once

#include <sstream>
#include <string>

#include "console/Commands.hpp"

// Console читает строку и вызывает нужную комманду
class Console {
 private:
  Commands commands_;

  bool ExecuteCommand(const std::string& line);
  void ParseAdd(std::istringstream& iss);
  void ParseDetails(std::istringstream& iss);

 public:
  void Run();

  // Run приносит строку. Весь разбор и ловля его ошибок живут здесь,
  // чтобы не тащить try/catch в main. false означает, что пора выходить.
  bool Execute(const std::string& line);
};
