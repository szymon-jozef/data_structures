#include <custom_concepts.hpp>
#include <sstream>

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

    Node(Y data) : data(data), left(nullptr), right(nullptr), parent(nullptr) {}
  };

  Node<T> *root;
  size_t size;

  bool insert_helper(Node<T> *node, const T &data) {
    T node_data = node->data;

    if (node_data == data) {
      return false;
    }

    if (data < node_data) {
      if (node->left) {
        return insert_helper(node->left, data);
      }

      Node<T> *new_node = new Node(data);
      new_node->parent = node;
      node->left = new_node;
      size++;
      return true;
    }

    if (data > node_data) {
      if (node->right) {
        return insert_helper(node->right, data);
      }

      Node<T> *new_node = new Node(data);
      new_node->parent = node;
      node->right = new_node;
      size++;
      return true;
    }

    return false;
  }

  void to_string_helper(Node<T> *node, std::stringstream &s) {
    if (!node) {
      s << 'N';
      return;
    }

    s << node->data;
    s << '(';
    to_string_helper(node->left, s);
    s << ',';
    to_string_helper(node->right, s);
    s << ')';
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

  bool is_empty() { return !root; }

  /// @brief Return the tree height
  size_t get_height() {
    // TODO!
    return -1;
  }

  std::string to_string() {
    std::stringstream s;
    s << '[';
    to_string_helper(root, s);
    s << ']';
    return s.str();
  }

  std::string to_string_summary() {
    // TODO!
    return "s";
  }
};
