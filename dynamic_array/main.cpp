#include "dynamic_array.hpp"
#include <chrono>
#include <cmath>
#include <print>

void simple_test() {
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

void test_pointers() {
  DynamicArray<int *> arr;
  for (size_t i = 0; i < 1000; i++) {
    arr.push_back(new int(rand() % 10001));
  }

  std::println("Array of pointers to int: {}", arr.as_string_summary());
  std::println("Clearing array with deallocation...");
  arr.clear(true);
}

struct TestStruct {
  int number;
  char super_character;

  TestStruct(int number, char character)
      : number(number), super_character(character) {}

  TestStruct() : number(0), super_character('0') {};

  auto operator<=>(const TestStruct &rhs) const {
    return number <=> rhs.number;
  }

  bool operator==(const TestStruct &rhs) const { return number == rhs.number; }

  friend std::ostream &operator<<(std::ostream &os, const TestStruct &t) {
    os << "{ number = " << t.number << ", character = " << t.super_character
       << "}";
    return os;
  }
};

void struct_test() {
  DynamicArray<TestStruct> arr;

  for (size_t i = 0; i < 1000; i++) {
    arr.push_back(TestStruct(rand() % 10001, (char)((rand() % 26) + 65)));
  }

  std::println("Test struct array: {}", arr.as_string_summary());
}

using fp_microseconds = std::chrono::duration<double, std::micro>;

void sort_test() {
  std::println("=== Sort test ===");
  DynamicArray<int> arr;

  const size_t DATA_SIZE = pow(10, 4);
  for (size_t i = 0; i < DATA_SIZE; i++) {
    arr.push_back(rand() % 10001);
  }

  std::println("Array before sort: {}", arr.as_string_summary());
  std::println("Sorting, please wait...");

  const auto T1 = std::chrono::high_resolution_clock::now();

  arr.bubble_sort();

  const auto T2 = std::chrono::high_resolution_clock::now();
  const auto DURATION = fp_microseconds(T2 - T1);
  std::println("Sorting took: {}", DURATION);

  std::println("Sorted arr: {}", arr.as_string());
}

void time_test() {
  std::println("=== Time test ===");

  DynamicArray<int> arr;

  const int ORDER = 7;
  const int DATA_SIZE = pow(10, ORDER);

  const auto T1 = std::chrono::high_resolution_clock::now();

  auto max_time_per_element = fp_microseconds(0);
  for (size_t i = 0; i < DATA_SIZE; i++) {
    int random_data = rand() % 1001;
    auto const T1_ELEMENT = std::chrono::high_resolution_clock::now();

    const bool IS_REALLOC = arr.push_back(random_data);

    auto const T2_ELEMENT = std::chrono::high_resolution_clock::now();
    const auto TIME_PER_ELEMENT = fp_microseconds(T2_ELEMENT - T1_ELEMENT);

    if (TIME_PER_ELEMENT > max_time_per_element) {
      std::println("New max time per element: {}", TIME_PER_ELEMENT);
      max_time_per_element = TIME_PER_ELEMENT;
    }

    if (IS_REALLOC) {
      std::println("Reallocation on element: {}!", i);
    }
  }

  const auto T2 = std::chrono::high_resolution_clock::now();
  const auto DURATION = fp_microseconds(T2 - T1);
  std::println("Array after insertion: {}", arr.as_string_summary());
  std::println("Amortized time: {}", DURATION / DATA_SIZE);
  std::println("Full time: {}", DURATION);
  arr.clear();
}

int main(void) { time_test(); }
