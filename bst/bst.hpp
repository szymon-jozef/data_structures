#include <custom_concepts.hpp>

template <typename T>
  requires Comparable<T> && Printable<T>
struct Node {
  T data;
  Node *parent;
  Node *left;
  Node *right;

  Node(T data) : data(data), left(nullptr), right(nullptr), parent(nullptr) {}
};

template <typename T>
  requires Comparable<T> && Printable<T>
class Bst {
  Node<T> *root;
  size_t size;

  bool insert_helper(Node<T> *root, const T &data) {
    T root_data = root->data;

    if (root_data == data) {
      return false;
    }

    if (data < root_data) {
      if (root->left) {
        return insert_helper(root->left, data);
      } else {
        Node<T> *new_node = new Node(data);
        new_node->parent = root;
        root->left = new_node;
        size++;
        return true;
      }
    }

    if (data > root_data) {
      if (root->right) {
        return insert_helper(root->right, data);
      } else {
        Node<T> *new_node = new Node(data);
        new_node->parent = root;
        root->right = new_node;
        size++;
        return true;
      }
    }
  }

public:
  Bst() = default;

  /// @brief Insert data into binary search tree
  /// @return `true` on success, `false` otherwise
  bool insert(const T &data) {
    if (is_empty()) {
      Node<T> *new_node = new Node(data);
      root = new_node;
      size++;
      return true;
    }

    return insert_helper(root, data);
  }

  /// @brief Search for a node with `data`
  /// @return Pointer to node containing `data`
  Node<T> *search(T data) {}

  /// @brief Remove given node
  void remove(Node<T> *node) {}

  /// @brief Remove every node from the tree
  void clear() {}

  bool is_empty() { return root; }

  /// @brief Return the tree height
  size_t get_height() {}

  std::string to_string() {}
  std::string to_string_summary() {}
};
