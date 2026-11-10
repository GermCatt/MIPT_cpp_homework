#include <iostream>

#include "console/Console.hpp"

#ifdef _WIN32
// Чтобы русский в консоли не превращался в клинопись при сборке под виндой
#include <windows.h>
#endif

int main() {
  // Чтобы русский в консоли не превращался в клинопись при сборке под виндой
#ifdef _WIN32
  SetConsoleCP(CP_UTF8);
  SetConsoleOutputCP(CP_UTF8);
#endif
  Console console;
  console.Run();
}
