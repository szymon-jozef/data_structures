#include <bst.hpp>
#include <print>

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
  std::println("{}", bst.to_string());
}
