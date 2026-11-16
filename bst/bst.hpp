#include <custom_concepts.hpp>
#include <ostream>
#include <sstream>

namespace bst {

enum class TraverseOrder { Inorder, Preorder, Postorder };

template <typename T>
  requires Comparable<T> && Printable<T>
class Bst {

  template <typename Y>
    requires Comparable<Y> && Printable<Y>
  struct Node {
    T data;
    Node *parent;
    Node *left;
    Node *right;
    size_t index;

    Node(Y data, size_t index)
        : data(data), parent(nullptr), left(nullptr), right(nullptr),
          index(index) {}

    friend std::ostream &operator<<(std::ostream &os, const Node &n) {
      os << '[' << n.data << "{" << n.index << "}"
         << "| Parent: " << (n.parent ? "Y" : "N")
         << "| Left: " << (n.left ? "Y" : "N")
         << "| Right: " << (n.right ? "Y" : "N") << ']';

      return os;
    }
  };

  Node<T> *root;
  size_t size;
  size_t current_node_index = 0;

  bool insert_helper(Node<T> *node, const T &data) {
    T node_data = node->data;

    if (node_data == data) {
      return false;
    }

    if (data < node_data) {
      if (node->left) {
        return insert_helper(node->left, data);
      }

      Node<T> *new_node = new Node(data, current_node_index++);
      new_node->parent = node;
      node->left = new_node;
      size++;
      return true;
    }

    if (data > node_data) {
      if (node->right) {
        return insert_helper(node->right, data);
      }

      Node<T> *new_node = new Node(data, current_node_index++);
      new_node->parent = node;
      node->right = new_node;
      size++;
      return true;
    }

    return false;
  }

  void to_string_helper(Node<T> *node, std::stringstream &s,
                        const TraverseOrder &order) {
    switch (order) {
    case TraverseOrder::Preorder:
      if (!node) {
        s << 'N';
        return;
      }

      s << node->data << '{' << node->index << '}';
      s << '(';
      to_string_helper(node->left, s, order);
      s << ',';
      to_string_helper(node->right, s, order);
      s << ')';
      break;
    case TraverseOrder::Inorder:
      if (!node) {
        return;
      }

      to_string_helper(node->left, s, order);
      s << node->data << '{' << node->index << '}';
      s << ',';
      to_string_helper(node->right, s, order);
      break;
    case TraverseOrder::Postorder:
      if (!node) {
        return;
      }

      to_string_helper(node->left, s, order);
      to_string_helper(node->right, s, order);
      s << node->data << '{' << node->index << '}';
      s << ',';
      break;
    }
  }

  Node<T> *search_helper(Node<T> *node, const T &data) const {
    if (!node) {
      return nullptr;
    }

    if (node->data == data) {
      return node;
    }

    if (node->data < data) {
      return search_helper(node->right, data);
    }

    if (node->data > data) {
      return search_helper(node->left, data);
    }
  }

public:
  Bst() = default;

  Bst(Bst &bst) = delete;
  Bst operator=(Bst &bst) = delete;

  /*
  ~Bst() { clear(); }
  */

  /// @brief Insert data into binary search tree
  /// @return `true` on success, `false` if `data` is a duplicate
  bool insert(const T data) {
    if (is_empty()) {
      Node<T> *new_node = new Node(data, current_node_index++);
      root = new_node;
      size++;
      return true;
    }

    return insert_helper(root, data);
  }

  /// @brief Search for a node with `data`
  /// @return Pointer to node containing `data`
  Node<T> *search(const T &data) const { return search_helper(root, data); }

  /// @brief Remove given node
  void remove(Node<T> *node) {}

  /// @brief Remove every node from the tree
  void clear() {}

  bool is_empty() { return !root; }

  /// @brief Return the tree height
  size_t get_height() {
    // TODO!
    return -1;
  }

  std::string to_string(const TraverseOrder &traverse_order) {
    std::stringstream s;
    s << '[';
    to_string_helper(root, s, traverse_order);
    s << ']';
    return s.str();
  }

  std::string to_string_summary() {
    // TODO!
    return "s";
  }
};
} // namespace bst
