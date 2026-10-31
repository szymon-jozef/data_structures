#include "dynamic_array.hpp"
#include <print>

int main(void) {
  DynamicArray<int> t;
  for (size_t i = 0; i < 1000; i++) {
    t.push_back(i);
  }

  std::println("Table: {}", t.as_string_summary());
  std::println("Element number 5: {}", t[5]);
  std::println("Change element 5 to 67");
  t[5] = 67;
  std::println("Table: {}", t.as_string());
  t.clear();
  std::println("Cleared array: {}", t.as_string());
}
