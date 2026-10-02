#include <iostream>
#include <map>
#include <string>

void deposit(int& s);
void withdraw(int& s);
void balance(int& s);
void reset(int& s);
void split(int& s);
void except(int& s);
void (*get_or(const std::map<std::string, void (*)(int&)>& cmdlst,
              const std::string key))(int&);

int main() {
  std::map<std::string, void (*)(int&)> commandMap = {{"deposit", deposit},
                                                      {"withdraw", withdraw},
                                                      {"balance", balance},
                                                      {"reset", reset},
                                                      {"split", split}};

  int s = 0;
  std::string command;
  while (std::cin >> command, command != "stop") {
    void (*usercmd)(int&) = get_or(commandMap, command);
    usercmd(s);
  }
  return 0;
}

void except(int& s) {
  std::cout << "error\n";
  return;
}

void deposit(int& s) {
  int k;
  std::cin >> k;
  if (std::cin.fail()) {
    std::cin.clear();
    std::cin.ignore();
    return except(s);
  }
  if (k <= 0) {
    return except(s);
  }
  s += k;
  return;
}

void withdraw(int& s) {
  int k;
  std::cin >> k;
  if (std::cin.fail()) {
    std::cin.clear();
    std::cin.ignore();
    return except(s);
  }
  if (k <= 0 || k > s) {
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

void (*get_or(const std::map<std::string, void (*)(int&)>& cmdlst,
              const std::string key))(int&) {
  auto it = cmdlst.find(key);
  if (it != cmdlst.end()) {
    return it->second;
  }
  return except;
}

void split(int& s) {
  int k;
  std::cin >> k;
  if (std::cin.fail()) {
    std::cin.clear();
    std::cin.ignore();
    return except(s);
  }
  if (s % k == 0 && k > 0) {
    s /= k;
    return;
  }
  return except(s);
}