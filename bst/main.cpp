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
}
