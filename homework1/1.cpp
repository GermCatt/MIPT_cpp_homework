#include <iomanip>
#include <iostream>

double Mean(const int* arr, int n) {
  int s = 0;
  for (int i = 0; i < n; ++i) {
    s += arr[i];
  }
  return static_cast<double>(s) / n;
}

void MinMax(const int* arr, int n, int& minimum, int& maximum) {
  minimum = arr[0];
  maximum = arr[0];
  for (int i = 1; i < n; ++i) {
    minimum = minimum > arr[i] ? arr[i] : minimum;
    maximum = maximum < arr[i] ? arr[i] : maximum;
  }
}

int UnderZero(const int* arr, int n) {
  int under_zero = 0;
  for (int i = 0; i < n; ++i) {
    under_zero += arr[i] < 0 ? 1 : 0;
  }
  return under_zero;
}

int main() {
  int n;
  std::cin >> n;
  int* temperatures = new int[n];
  for (int i = 0; i < n; ++i) {
    std::cin >> temperatures[i];
  }
  double mean = Mean(temperatures, n);
  int minimum;
  int maximum;
  MinMax(temperatures, n, minimum, maximum);
  int under_zero = UnderZero(temperatures, n);
  std::cout << std::fixed << std::setprecision(1) << mean << std::endl;
  std::cout << minimum << " " << maximum << std::endl;
  std::cout << under_zero << std::endl;
  delete[] temperatures;
}