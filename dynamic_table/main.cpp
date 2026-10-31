#include "dynamic_table.hpp"
#include <print>

int main(void) {
  DynamicTable<int> t;
  for (size_t i = 0; i < 1000; i++) {
    t.push_back(i);
  }

  std::println("Table: {}", t.as_string());
  std::println("Element number 5: {}", t[5]);
  std::println("Change element 5 to 67");
  t[5] = 67;
  std::println("Table: {}", t.as_string());
}
