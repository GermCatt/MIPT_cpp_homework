#include <iostream>
#include <iomanip>

int main() {
  int n;
  std::cin >> n;
  int s;
  std::cin >> s;
  int maximum = s;
  int minimum = s;
  int under_zero = s < 0 ? 1 : 0;
  for (int i = 0; i < n - 1; ++i) {
    int temp;
    std::cin >> temp;
    s += temp;
    maximum = maximum < temp ? temp : maximum;
    minimum = minimum > temp ? temp : minimum;
    under_zero += temp < 0 ? 1 : 0;
  }
  float mean = (float) s / n;
  std::cout << std::fixed << std::setprecision(1) << mean << std::endl;
  std::cout << minimum << " " << maximum << std::endl;
  std::cout << under_zero << std::endl;
}
