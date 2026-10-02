#include <iostream>

using Handler = void (*)(int&);

struct Command {
  const char* name;
  Handler run;
};

void deposit(int& s);
void withdraw(int& s);
void balance(int& s);
void reset(int& s);
void split(int& s);
void except(int& s);
bool equal(const char*, const char*);
Handler get_or(const Command*, const char*, int);

int main() {
  Command commandMap[] = {{"deposit", deposit},
                          {"withdraw", withdraw},
                          {"balance", balance},
                          {"reset", reset},
                          {"split", split}};
  const int count = sizeof(commandMap) / sizeof(commandMap[0]);

  int s = 0;
  char command[16];

  while (true) {
    std::cin.width(sizeof(command));
    if (!(std::cin >> command) || equal(command, "stop")) {
      break;
    }
    Handler usercmd = get_or(commandMap, command, count);
    usercmd(s);
  }
  return 0;
}

void except(int&) {
  std::cout << "error\n";
  std::cin.clear();
  char c;
  while (std::cin.get(c) && c != '\n') {
  }
  return;
}

void deposit(int& s) {
  int k;
  if (!(std::cin >> k) || k <= 0) {
    return except(s);
  }
  s += k;
  return;
}

void withdraw(int& s) {
  int k;
  if (!(std::cin >> k) || k <= 0 || k > s) {
    return except(s);
  }
  s -= k;
  return;
}

void balance(int& s) {
  std::cout << s << "\n";
  return;
}

void reset(int& s) {
  s = 0;
  return;
}

Handler get_or(const Command* cmdlst, const char* key, const int size) {
  for (int i = 0; i < size; ++i) {
    if (equal(cmdlst[i].name, key)) {
      return cmdlst[i].run;
    }
  }
  return except;
}

void split(int& s) {
  int k;
  if (!(std::cin >> k) || k <= 0 || s % k != 0) {
    return except(s);
  }
  s /= k;
  return;
}

bool equal(const char* a, const char* b) {
  int i = 0;
  while (a[i] != '\0' && a[i] == b[i]) {
    ++i;
  }
  return a[i] == b[i];
}