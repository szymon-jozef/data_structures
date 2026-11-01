#include "linked_list.hpp"
#include <chrono>
#include <cmath>
#include <ostream>
#include <print>

void search_for_value_and_display(LinkedList<int> &l, int value) {
  auto found_index = l.find_index(value);
  std::println("Searching for value {} in the list...", value);
  if (found_index) {
    std::println("Found at index: {}", found_index.value());
  } else {
    std::println("Not found");
  }
}

void simple_test() {
  { // not sorted
    std::println("=== Basic operations on linked list ===");
    LinkedList<int> l;
    std::println("Empty list: {}", l.to_string());

    l.push_front(4);
    l.push_front(5);
    l.push_front(6);
    l.push_front(1);
    l.push_front(0);
    l.push_front(-1);
    std::println("List after pushing front: {}", l.to_string());

    l.pop_found(5);
    std::println("List after removing 5: {}", l.to_string());

    l.push_back(2);
    l.push_back(3);
    std::println("List after pushing back: {}", l.to_string());

    std::println("Insert 67 after 2");
    l.insert_after(67, 2);
    std::println("{}", l.to_string());

    std::println("Last element poped: {}", l.pop_back());
    std::println("{}", l.to_string());

    std::println("First element poped: {}", l.pop_front());
    std::println("{}", l.to_string());

    search_for_value_and_display(l, 0);
    search_for_value_and_display(l, 67);

    std::println("Get 2nd element ptr");
    const int index = 1;
    auto element = l[index];
    if (!element) {
      std::println("It is null");
    } else {
      std::println("2nd element exists and it's value is: {}",
                   element->get_data());
      std::println("Change value of the 2nd element to 67");
      element->set_data(67);
      std::println("List after change: {}", l.to_string());
      l.pop_nth(index);
      std::println("List after poping this element by index: {}",
                   l.to_string());
    }
  }

  { // list of pointer
    std::println("=== List of pointers ===");
    LinkedList<int *> pointer_list;
    pointer_list.push_back(new int(1));
    pointer_list.push_back(new int(2));
    pointer_list.push_back(new int(3));
    std::println("List of pointers: {}", pointer_list.to_string());
    pointer_list.clear(true);
  }

  { // sorted
    std::println("=== Sorted list of int ===");
    LinkedList<int> sorted_list;
    sorted_list.push_sorted(-1);
    sorted_list.push_sorted(-5);
    sorted_list.push_sorted(-10);
    sorted_list.push_sorted(50);
    sorted_list.push_sorted(60);
    sorted_list.push_sorted(10);
    sorted_list.push_sorted(67);
    sorted_list.push_sorted(-67);
    std::println("Sorted list: {}", sorted_list.to_string());
  }
}

struct TestStruct {
  int field;

  auto operator<=>(const TestStruct &t) const { return field <=> t.field; }
  bool operator==(const TestStruct &t) const { return field == t.field; }

  friend std::ostream &operator<<(std::ostream &os, const TestStruct &t) {
    os << t.field;
    return os;
  }
};

void test_custom_type() {
  std::println("=== Test struct ===");
  LinkedList<TestStruct> ll;
  ll.push_sorted(TestStruct{5});
  ll.push_sorted(TestStruct{1});
  ll.push_sorted(TestStruct{2});
  ll.push_sorted(TestStruct{10});
  std::println("Ordered list of tests structs: {}", ll.to_string());
}

using fp_microseconds = std::chrono::duration<double, std::micro>;

template <typename T>
void test_pushing(LinkedList<T> &ll, const size_t data_size) {
  std::println("\n=== Pushing ===");
  auto const t1 = std::chrono::high_resolution_clock::now();

  for (size_t i = 0; i < data_size; i++) {
    ll.push_back(rand() % 10001);
  }

  auto const t2 = std::chrono::high_resolution_clock::now();
  auto const duration = fp_microseconds(t2 - t1);

  std::println("List after pushing: {}", ll.to_string_summary());
  std::println("Duration: {}", duration);
  std::println("Avg time: {}", duration / data_size);
}

template <typename T>
void test_popping(LinkedList<T> &ll, const size_t search_amount) {
  std::println("\n=== Popping ===");

  auto const t1 = std::chrono::high_resolution_clock::now();

  for (size_t i = 0; i < search_amount; i++) {
    int random = rand() % 10001;
    ll.pop_found(random);
  }

  auto const t2 = std::chrono::high_resolution_clock::now();
  auto const duration = fp_microseconds(t2 - t1);

  std::println("List after removing: {}", ll.to_string_summary());
  std::println("Duration: {}", duration);
  std::println("Avg remove time: {}", duration / search_amount);
}

void time_test() {
  std::println("====== Time tests ======");
  const int MAX_ORDER = 6;
  LinkedList<int> ll;
  srand(time(nullptr));

  for (int current_order = 1; current_order <= MAX_ORDER; current_order++) {
    std::println("Current order: {}", current_order);

    const size_t data_size = std::pow(10, current_order);
    test_pushing<int>(ll, data_size);

    const size_t search_amount = std::pow(10, 4);
    test_popping<int>(ll, search_amount);

    ll.clear();
  }
}

int main(void) { time_test(); }
