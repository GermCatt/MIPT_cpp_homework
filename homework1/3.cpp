#include <iostream>

constexpr int WIDTH = 10;
constexpr int HEIGHT = 5;

using Display = char[HEIGHT][WIDTH];
using Handler = bool (*)(Display&);

struct Command {
  const char* name;
  Handler run;
};

using Dict = Command[4];

bool print(Display&);
bool clear(Display&);
bool ConsolePut(Display&);
bool ConsoleText(Display&);
bool put(Display&, int, int, char);
bool text(Display&, int, int, const char*);
bool write_chars(Display&, int, int, const char*, int);
bool equal(const char*, const char*);
Handler get_or(const Dict&, const char*);

int main() {
  Display display;
  clear(display);

  Dict commandMap = {
      {"print", print}, {"put", ConsolePut},
      {"text", ConsoleText}, {"clear", clear}};

  char userCommand[6];

  while (true) {
    std::cin.width(sizeof(userCommand));
    if (!(std::cin >> userCommand) || equal(userCommand, "quit")) {
      break;
    }
    auto run = get_or(commandMap, userCommand);
    if (!run(display)) {
      std::cout << "error\n";
    }
  }
}

bool print(Display& display) {
  for (auto& row : display) {
    for (auto symbol : row) {
      std::cout << symbol;
    }
    std::cout << "\n";
  }
  return true;
}

bool clear(Display& display) {
  for (auto& row : display) {
    for (auto& symbol : row) {
      symbol = ' ';
    }
  }
  return true;
}

bool ConsolePut(Display& display) {
  int x, y;
  char c;
  if (!(std::cin >> x >> y >> c)) {
    return false;
  }
  return put(display, x, y, c);
}

bool ConsoleText(Display& display) {
  int x, y;
  char t[WIDTH + 1];
  if (!(std::cin >> x >> y)) {
    return false;
  }
  std::cin.width(sizeof(t));
  if (!(std::cin >> t)) {
    return false;
  }
  return text(display, x, y, t);
}

bool put(Display& display, const int x, const int y, const char c) {
  return write_chars(display, x, y, &c, 1);
}

bool text(Display& display, const int x, const int y, const char* t) {
  int size = 0;
  while (t[size] != '\0') {
    ++size;
  }
  return write_chars(display, x, y, t, size);
}

bool write_chars(Display& display, const int x, const int y,
                 const char* t, const int size) {
  if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT ||
      size < 0 || size > WIDTH - x) {
    return false;
  }
  for (int i = 0; i < size; ++i) {
    display[y][x + i] = t[i];
  }
  return true;
}

bool equal(const char* a, const char* b) {
  int i = 0;
  while (a[i] != '\0' && a[i] == b[i]) {
    ++i;
  }
  return a[i] == b[i];
}

Handler get_or(const Dict& cmdlst, const char* key) {
  for (const auto& command : cmdlst) {
    if (equal(command.name, key)) {
      return command.run;
    }
  }
  return +[](Display&) { return false; };
}
