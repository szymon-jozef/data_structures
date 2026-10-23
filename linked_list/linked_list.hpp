#include <cassert>
#include <concepts>
#include <cstddef>
#include <optional>
#include <sstream>
#include <string>

template <std::equality_comparable T> class Node {
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

template <typename T> class LinkedList {
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

public:
  LinkedList() : head(nullptr), tail(nullptr), size(0) {}
  LinkedList(const LinkedList &) = delete;
  LinkedList &operator=(const LinkedList) = delete;
  ~LinkedList() { clear(); }

  // === Inserting ===

  /// Add new node at the end of the list.
  /// This method allocates on the heap
  void push_back(T data) {
    if (!tail && !head) {
      init_list(data);
      return;
    }

    assert(("Head or tail is a nullptr. This shouldn't happen", head && tail));

    Node<T> *new_node = new Node(data);

    new_node->set_prev(tail);
    tail->set_next(new_node);
    tail = new_node;

    size++;
  }

  /// Add new node at the beginning of the list.
  /// This method allocates on the heap
  void push_front(T data) {
    if (!tail && !head) {
      init_list(data);
      return;
    }

    assert(("Head or tail is a nullptr. This shouldn't happen", head && tail));

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
    const auto found_index = find(after);
    if (!found_index) {
      return;
    }

    Node<T> *found_node = (*this)[found_index.value()];
    Node<T> *after_found_node = found_node->get_next();
    Node<T> *new_node = new Node(data);

    new_node->set_prev(found_node);
    new_node->set_next(after_found_node);

    found_node->set_next(new_node);
    after_found_node->set_prev(new_node);
  }

  /// Push `data` into the list, ensuring sorted order.
  /// **Shouldn't** be mixed with other pushing methods.
  ///
  /// This method allocates on the heap
  void push_sorted(T data) {
    if (!tail && !head) {
      init_list(data);
      return;
    }

    assert(head && tail);

    if (data > tail->get_data()) {
      this->push_back(data);
      return;
    }

    if (data < head->get_data()) {
      this->push_front(data);
      return;
    }

    Node<T> *n = head;

    do {
      if (n->get_data() < data && data < n->get_next()->get_data()) {
        Node<T> *before_node = n;
        Node<T> *new_node = new Node(data);
        Node<T> *after_node = n->get_next();

        before_node->set_next(new_node);

        new_node->set_prev(before_node);
        new_node->set_next(after_node);

        after_node->set_prev(new_node);

        return;
      }

      if (n == tail) {
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
    assert(("Head and tail are nullptrs. This shouldn't happen", head && tail));

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
    assert(("Head and tail are nullptrs. This shouldn't happen", head && tail));

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
    // TODO!()
  }

  // === Traversing ===

  /// Find index of node with specified `data`
  /// If two nodes contains the same `data`, it will return the first occurrence
  std::optional<size_t> find(T data) {
    Node<T> *n = head;

    for (size_t i = 0; i < size; i++) {
      if (n->get_data() == data) {
        return i;
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
  void clear() {
    if (!head && !tail) {
      return;
    }

    assert(
        ("List is initialized, but head or tail isn't set. The list is broken!",
         head || tail));

    size_t size = this->size;
    for (size_t i = 0; i < size; i++) {
      pop_front();
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
      s << n->get_data();

      if (n->get_next()) {
        s << ',';
      }

    } while ((n = n->get_next()));

    s << ']';

    return s.str();
  }
};
