#pragma once
#include <algorithm>
#include <cstddef>
#include <custom_concepts.hpp>
#include <optional>
#include <ostream>
#include <sstream>
#include <string>
#include <type_traits>
#include <utility>

namespace bst {

enum class TraverseOrder { Inorder, Preorder, Postorder };

template <typename T>
  requires Comparable<T> && Printable<T>
class Bst {

  static const auto &unwrap(const T &data) {
    if constexpr (std::is_pointer_v<T>) {
      return *data;
    } else {
      return data;
    }
  }

  static bool lt(const T &a, const T &b) { return unwrap(a) < unwrap(b); }

  struct Node {
    T data;
    Node *parent;
    Node *left;
    Node *right;
    size_t index;

    Node(T data, size_t index)
        : data(std::move(data)), parent(nullptr), left(nullptr), right(nullptr),
          index(index) {}

    friend std::ostream &operator<<(std::ostream &os, const Node &n) {
      os << '[' << unwrap(n.data) << "{" << n.index << "}"
         << "| Parent: " << (n.parent ? "Y" : "N")
         << "| Left: " << (n.left ? "Y" : "N")
         << "| Right: " << (n.right ? "Y" : "N") << ']';

      return os;
    }
  };

  Node *root = nullptr;
  size_t size = 0;
  size_t current_node_index = 0;

  size_t count_height_helper(Node *node, size_t current_height) const {
    if (!node)
      return 0;

    const size_t h_left = count_height_helper(node->left, current_height + 1);
    const size_t h_right = count_height_helper(node->right, current_height + 1);
    return std::max(std::max(h_left, h_right), current_height);
  }

  size_t count_height() const {
    if (!root)
      return 0;

    return count_height_helper(root, 0);
  }

  bool insert_helper(Node *node, const T &data) {
    const T node_data = node->data;

    if (lt(data, node_data)) {
      if (node->left) {
        return insert_helper(node->left, data);
      }

      Node *new_node = new Node(data, current_node_index++);
      new_node->parent = node;
      node->left = new_node;
      size++;
      return true;
    }

    if (lt(node_data, data)) {
      if (node->right) {
        return insert_helper(node->right, data);
      }

      Node *new_node = new Node(data, current_node_index++);
      new_node->parent = node;
      node->right = new_node;
      size++;
      return true;
    }

    return false;
  }

  void to_string_helper(Node *node, std::stringstream &s,
                        const TraverseOrder &order) const {
    switch (order) {
    case TraverseOrder::Preorder:
      if (!node) {
        s << 'N';
        return;
      }

      s << unwrap(node->data) << '{' << node->index << '}';
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
      s << unwrap(node->data) << '{' << node->index << '}';
      s << ',';
      to_string_helper(node->right, s, order);
      break;
    case TraverseOrder::Postorder:
      if (!node) {
        return;
      }

      to_string_helper(node->left, s, order);
      to_string_helper(node->right, s, order);
      s << unwrap(node->data) << '{' << node->index << '}';
      s << ',';
      break;
    }
  }

  Node *search_helper(Node *node, const T &data) const {
    if (!node) {
      return nullptr;
    }

    if (lt(node->data, data)) {
      return search_helper(node->right, data);
    }

    if (lt(data, node->data)) {
      return search_helper(node->left, data);
    }

    return node;
  }

  /// @brief Replaces one subtree as a child of its parent with another subtree
  void transplant(Node *to_replace, Node *replacement) {
    if (to_replace == root) {
      root = replacement;
    } else if (to_replace == to_replace->parent->left) {
      to_replace->parent->left = replacement;
    } else if (to_replace == to_replace->parent->right) {
      to_replace->parent->right = replacement;
    }

    if (replacement != nullptr) {
      replacement->parent = to_replace->parent;
    }
  }

  static Node *find_minimum(Node *node) {
    if (node->left == nullptr) {
      return node;
    } else {
      return find_minimum(node->left);
    }
  }

  static Node *find_maximum(Node *node) {
    if (node->right == nullptr) {
      return node;
    } else {
      return find_maximum(node->right);
    }
  }

  void remove_node(Node *node, bool should_free = false) {
    if constexpr (std::is_pointer_v<T>) {
      if (should_free) {
        delete node->data;
      }
    }
    delete node;
  }

public:
  Bst() = default;

  Bst(const Bst &bst) = delete;
  Bst &operator=(const Bst &bst) = delete;

  ~Bst() { clear(); }

  /// @brief Insert data into binary search tree
  /// @return `true` on success, `false` if `data` is a duplicate
  bool insert(const T &data) {
    if (is_empty()) {
      Node *new_node = new Node(data, current_node_index++);
      root = new_node;
      size++;
      return true;
    }

    return insert_helper(root, data);
  }

  /// @brief Search for a node with `data`
  /// @return Pointer to node containing `data`
  Node *search(const T &data) const { return search_helper(root, data); }

  /// @brief Remove given node
  void remove(Node *node, bool free_memory = false) {
    if (!node) { // nothing to remove
      return;
    }

    // first case -> node has only right child
    if (node->left == nullptr) {
      transplant(node, node->right);
      remove_node(node);
      size--;
      return;
    } else if (node->right ==
               nullptr) { // second case -> node has only left child
      transplant(node, node->left);
      remove_node(node);
      size--;
      return;
    } else { // third case -> node has two children
      Node *min = find_minimum(node->right);
      if (min->parent != node) {
        transplant(min, min->right);
        min->right = node->right;
        min->right->parent = min;
      }

      transplant(node, min);
      min->left = node->left;
      min->left->parent = min;

      remove_node(node);
      size--;
    }
  }

  /// @brief Remove every node from the tree
  void clear(bool free_memory = false) {
    while (root) {
      remove(root, free_memory);
    }
    current_node_index = 0;
    size = 0;
  }

  bool is_empty() const { return !root; }

  /// @brief Return the tree height
  size_t get_height() const { return count_height(); }
  /// @brief Return the tree size (amount of nodes)
  size_t get_size() const { return size; }

  std::string to_string(const TraverseOrder &traverse_order) const {
    std::stringstream s;
    s << '[';
    to_string_helper(root, s, traverse_order);
    s << ']';
    return s.str();
  }

  std::optional<T> get_max() const {
    if (root)
      return find_maximum(root)->data;
    else
      return std::nullopt;
  }

  std::optional<T> get_min() const {
    if (root)
      return find_minimum(root)->data;
    else
      return std::nullopt;
  }

  std::string to_string_summary() const {
    std::stringstream s;
    s << '[';
    s << "Size: " << size << " | ";
    s << "Height: " << get_height();
    s << ']';
    return s.str();
  }
};
} // namespace bst
