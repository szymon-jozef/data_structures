#include <bst.hpp>
#include <cassert>
#include <print>
#include <sstream>

using namespace bst;

template <typename T> void test_remove_value(Bst<T> &bst, const T &value) {
  auto found = bst.search(value);
  if (!found) {
    std::println("\nCouldn't find value: {} in the bst", value);
    return;
  }

  std::println("\nBst tree before removing value {}: {}", value,
               bst.to_string(TraverseOrder::Inorder));
  bst.remove(found);
  std::println("Bst tree after removing value {}: {}", value,
               bst.to_string(TraverseOrder::Inorder));
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

int main(void) {
  Bst<int> bst;
  bst.insert(1);
  bst.insert(2);
  bst.insert(3);
  bst.insert(-1);
  bst.insert(-5);
  bst.insert(-10);
  bst.insert(100);
  bst.insert(200);
  std::println("Bst tree: ");
  std::println("Inorder: \n{}", bst.to_string(TraverseOrder::Inorder));
  std::println("Preorder: \n{}", bst.to_string(TraverseOrder::Preorder));
  std::println("Postorder: \n{}", bst.to_string(TraverseOrder::Postorder));
  const auto found = bst.search(3);
  if (found) {
    std::stringstream s;
    s << *found;
    std::println("Search for node with value 3 and found: {}", s.str());
  } else {
    std::println("Value 3 not found in the bst!");
  }
  std::println("Tree height: {}", bst.get_height());
  std::println("=== Test removing ===\n");
  std::println("Tree summary: {}", bst.to_string_summary());

  size_t current_size = bst.get_size();
  test_remove_value<int>(bst, 1);
  assert(bst.get_size() == current_size - 1);
  current_size--;

  test_remove_value<int>(bst, 3);
  assert(bst.get_size() == current_size - 1);
  current_size--;

  test_remove_value<int>(bst, 200);
  assert(bst.get_size() == current_size - 1);
  current_size--;

  test_remove_value<int>(bst, -5);
  assert(bst.get_size() == current_size - 1);
  current_size--;

  test_remove_value<int>(bst, -10);
  std::println("Tree summary: {}", bst.to_string_summary());

  std::println("=== Test struct ===");
  Bst<TestStruct> bst_struct;
  bst_struct.insert(TestStruct(2, 'c'));
  bst_struct.insert(TestStruct(5, 'a'));
  bst_struct.insert(TestStruct(-1, 'd'));
  std::println("Test struct bst: {}",
               bst_struct.to_string(TraverseOrder::Inorder));

  std::println("=== Ptr struct ===");
  Bst<int *> bst_ptr;
  bst_ptr.insert(new int(2));
  bst_ptr.insert(new int(4));
  bst_ptr.insert(new int(1));
  std::println("Bst of ptr to int: {}",
               bst_ptr.to_string(TraverseOrder::Inorder));
}
