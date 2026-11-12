#include "console/ConsoleOutput.hpp"

#include <cstdlib>
#include <iostream>
#include <string_view>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

// В целом, весь этот файл существует для того, чтобы в консоли выводить
// разноцветные тексты и при этом, при перенаправлении потока в файл, он не
// писал туда коды цвентного консольного текста
namespace {
bool EnableColor() {
  const char* noColor = std::getenv("NO_COLOR");
  if (noColor && *noColor) {
    return false;
  }
  const char* term = std::getenv("TERM");
  if (term && std::string_view(term) == "dumb") {
    return false;
  }

#ifdef _WIN32
  const HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
  DWORD mode = 0;
  // У файла или канала нет режима консоли: управляющие коды туда не пишем.
  return GetConsoleMode(output, &mode) &&
         SetConsoleMode(output, mode | ENABLE_PROCESSED_OUTPUT |
                                    ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#else
  return isatty(STDOUT_FILENO) != 0;
#endif
}
void PrintRedLine(std::string_view prefix, std::string_view message) {
  const bool color = EnableColor();
  if (color) {
    std::cout << "\x1b[31m";
  }
  std::cout << prefix << message;
  if (color) {
    std::cout << "\x1b[0m";
  }
  std::cout << '\n';
}
}  // namespace

void ConsoleOutput::Error(std::string_view message) {
  PrintRedLine("error: ", message);
}

void ConsoleOutput::Removed() { PrintRedLine("", "already removed"); }

void ConsoleOutput::Message(std::string_view message) {
  std::cout << message << '\n';
}
