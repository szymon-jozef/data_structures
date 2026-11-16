#include <bst.hpp>
#include <print>

using namespace bst;

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
  std::println("Tree height: {}", bst.get_height());
}
