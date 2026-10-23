#include "linked_list.hpp"
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

int main(void) {
  { // not sorted
    LinkedList<int> l;
    std::println("Empty list: {}", l.to_string());

    l.push_front(1);
    l.push_front(0);
    l.push_front(-1);
    std::println("List after pushing front: {}", l.to_string());

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

  { // sorted
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
