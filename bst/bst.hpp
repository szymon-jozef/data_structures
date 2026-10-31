#include <concepts>
#include <ostream>

template <typename T>
concept Printable = requires(std::ostream &os, const T &t) {
  { os << t } -> std::same_as<std::ostream &>;
};

template <typename T>
concept Comparable = std::totally_ordered<std::remove_pointer_t<T>>;

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

public:
  Bst() = default;

  /// @brief Insert data into binary search tree
  void insert(T data) {}

  /// @brief Search for a node with `data`
  /// @return Pointer to node containing `data`
  Node<T> *search(T data) {}

  /// @brief Remove given node
  void remove(Node<T> *node) {}

  /// @brief Remove every node from the tree
  void clear() {}

  /// @brief Return the tree height
  size_t get_height() {}

  std::string to_string() {}
  std::string to_string_summary() {}
};
