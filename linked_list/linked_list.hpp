#include <cassert>
#include <concepts>
#include <cstddef>
#include <optional>
#include <sstream>
#include <string>
#include <type_traits>

template <typename T>
concept Comparable = std::totally_ordered<std::remove_pointer_t<T>>;

template <Comparable T> class Node {
  T data;
  Node *next;
  Node *prev;

public:
  Node(T data) : data(data), next(nullptr), prev(nullptr) {}

  void set_next(Node *next) { this->next = next; }
  void set_prev(Node *prev) { this->prev = prev; }
  void set_data(T data) { this->data = data; }

  Node *get_next() const { return next; }
  Node *get_prev() const { return prev; }
  T get_data() const { return data; }
};

template <Comparable T> class LinkedList {
  Node<T> *head;
  Node<T> *tail;
  size_t size;

  /// Create the first node and set is as the head and tail
  void init_list(T data) {
    Node<T> *n = new Node(data);
    head = n;
    tail = n;
    size++;
  }

  /// Return the copy of data in the last node, deallocate node and set head and
  /// tail as nullptr.
  T pop_last_element() {
    T data = tail->get_data();
    delete tail;
    head = nullptr;
    tail = nullptr;
    size = 0;
    return data;
  }

  static const auto &unwrap(const T &data) {
    if constexpr (std::is_pointer<T>::value) {
      return *data;
    } else {
      return data;
    }
  }

public:
  LinkedList() : head(nullptr), tail(nullptr), size(0) {}
  LinkedList(const LinkedList &) = delete;
  LinkedList &operator=(const LinkedList) = delete;
  ~LinkedList() { clear(); }

  // === Inserting ===

  /// Add new node at the end of the list.
  /// This method allocates on the heap
  void push_back(T data) {
    if (!this->is_initialized()) {
      init_list(data);
      return;
    }

    assert(("List is broken", !this->is_broken()));

    Node<T> *new_node = new Node(data);

    new_node->set_prev(tail);
    tail->set_next(new_node);
    tail = new_node;

    size++;
  }

  /// Add new node at the beginning of the list.
  /// This method allocates on the heap
  void push_front(T data) {
    if (!this->is_initialized()) {
      init_list(data);
      return;
    }

    assert(("List is broken", !this->is_broken()));

    Node<T> *new_node = new Node(data);
    new_node->set_next(head);
    head->set_prev(new_node);

    head = new_node;

    size++;
  }

  /// Insert `data` right after the first node containing `after` value
  ///
  /// If `after` is not found in the list, it does nothing
  ///
  /// This method allocates on the heap
  void insert_after(T data, T after) {
    const auto result = find_pointer(after);
    if (!result) {
      return;
    }

    if (result.value() == tail) {
      push_back(data);
      return;
    }

    Node<T> *found_node = result.value();
    Node<T> *after_found_node = found_node->get_next();
    Node<T> *new_node = new Node(data);

    new_node->set_prev(found_node);
    new_node->set_next(after_found_node);

    found_node->set_next(new_node);
    after_found_node->set_prev(new_node);

    size++;
  }

  /// Push `data` into the list, ensuring sorted order.
  /// **Shouldn't** be mixed with other pushing methods.
  ///
  /// This method allocates on the heap
  void push_sorted(T data) {
    // first element on the list
    if (!this->is_initialized()) {
      init_list(data);
      return;
    }
    // smaller than the first node
    if (unwrap(data) >= unwrap(tail->get_data())) {
      this->push_back(data);
      return;
    }

    // larger than the last element
    if (unwrap(data) <= unwrap(head->get_data())) {
      this->push_front(data);
      return;
    }

    Node<T> *n = head;

    // between other data
    do {
      if (unwrap(n->get_data()) <= unwrap(data) &&
          unwrap(data) <= unwrap(n->get_next()->get_data())) {
        Node<T> *before_node = n;
        Node<T> *new_node = new Node(data);
        Node<T> *after_node = n->get_next();

        before_node->set_next(new_node);

        new_node->set_prev(before_node);
        new_node->set_next(after_node);

        after_node->set_prev(new_node);

        size++;

        return;
      }

      if (n == tail) { // don't double check end
        return;
      }

    } while ((n = n->get_next()));
  }

  // === Removing ===

  /// Remove the last element of the list
  /// This method deallocates the last node
  ///
  /// @return Copy of data in the last node
  T pop_back() {
    assert(("List is broken", !this->is_broken()));

    if (head == tail) {
      return pop_last_element();
    }

    T data = tail->get_data();

    Node<T> *old_tail = tail;

    old_tail->get_prev()->set_next(nullptr);
    tail = old_tail->get_prev();

    delete old_tail;

    size--;
    return data;
  }

  /// Remove the first element of the list
  /// This method deallocates the first node
  ///
  /// @return Copy of data in the first node
  T pop_front() {
    assert(("List is broken", !this->is_broken()));

    if (head == tail) {
      return pop_last_element();
    }

    T data = head->get_data();

    Node<T> *old_head = head;

    old_head->get_next()->set_prev(nullptr);
    head = old_head->get_next();

    delete old_head;

    size--;
    return data;
  }

  /// Remove the nth element of the list.
  /// This method deallocates selected node
  ///
  /// @return Copy of data in the selected node
  T pop_nth(size_t index) {
    assert(("List is not initialized", is_initialized()));
    assert(("List is empty", !is_empty()));
    assert(("Index is out of bounds", index < size));

    if (index == 0) {
      return this->pop_front();
    }

    if (index == size - 1) {
      return this->pop_back();
    }

    Node<T> *n = (*this)[index];
    T data = n->get_data();

    Node<T> *before = n->get_prev();
    Node<T> *after = n->get_next();

    before->set_next(after);
    after->set_prev(before);

    delete n;

    size--;
    return data;
  }

  bool pop_found(T data) {
    Node<T> *found = this->find_pointer(data).value_or(nullptr);

    if (!found) {
      return false;
    }

    Node<T> *before = found->get_prev();
    Node<T> *after = found->get_next();
    if (!before) {
      pop_front();
      return true;
    }

    if (!after) {
      pop_back();
      return true;
    }

    before->set_next(after);
    after->set_prev(before);

    delete found;
    size--;
    return true;
  }

  // === Traversing ===

  /// Find index of node with specified `data`
  /// If two nodes contains the same `data`, it will return the first occurrence
  std::optional<size_t> find_index(T data) {
    Node<T> *n = head;

    for (size_t i = 0; i < size; i++) {
      if (n->get_data() == unwrap(data)) {
        return i;
      }

      n = n->get_next();
    }

    return std::nullopt;
  }

  std::optional<Node<T> *> find_pointer(T data) {
    Node<T> *n = head;

    for (size_t i = 0; i < size; i++) {
      if (n->get_data() == unwrap(data)) {
        return n;
      }

      n = n->get_next();
    }

    return std::nullopt;
  }

  Node<T> *operator[](size_t index) {
    assert(("Index is out of bounds", index < size));

    Node<T> *n = head;

    for (size_t i = 0; i < size; i++) {
      if (i == index) {
        return n;
      }

      n = n->get_next();
    }

    return nullptr;
  }

  const Node<T> *operator[](size_t index) const {
    assert(("Index is out of bounds", index < size));

    Node<T> *n = head;

    for (size_t i = 0; i < size; i++) {
      if (i == index) {
        return n;
      }

      n = n->get_next();
    }

    return nullptr;
  }

  // === Misc ===

  size_t get_size() const { return size; }

  /// Deallocate the list
  /// @param should_free_members If set to `true` and T is a pointer, it will
  /// free the resources it holds
  void clear(bool should_free_members = false) {
    if (this->is_empty()) {
      return;
    }

    assert(("List is broken", !this->is_broken()));

    size_t size = this->size;
    for (size_t i = 0; i < size; i++) {
      auto member = pop_front();
      if constexpr (std::is_pointer<T>::value) {
        if (should_free_members && member) {
          delete member;
        }
      }
    }
  }

  /// @return String representation of the linked list
  const std::string to_string() const {
    if (!head || !tail) {
      return "[]";
    }

    std::stringstream s;
    s << '[';

    Node<T> *n = head;

    do {
      T data = n->get_data();

      s << unwrap(data);

      if (n->get_next()) {
        s << ',';
      }

    } while ((n = n->get_next()));

    s << ']';

    return s.str();
  }

  /// @return True if `size` is 0
  bool is_empty() const { return size == 0; }

  /// @return True if `head` and `tail` are both set
  bool is_initialized() const { return head && tail; }

  /// Check if the list is broken
  /// @return True if `head` is valid and `tail` is a `nullptr` or `head` is a
  /// `nullptr` and tail is valid
  bool is_broken() const { return (head && !tail) || (!head && tail); }
};
