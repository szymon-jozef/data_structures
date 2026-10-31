#include <cassert>
#include <concepts>
#include <cstddef>
#include <cstring>
#include <sstream>

template <typename T>
concept Printable = requires(std::ostream &os, const T &t) {
  { os << t } -> std::same_as<std::ostream &>;
};

template <typename T>
concept Comparable = std::totally_ordered<std::remove_pointer_t<T>>;

template <typename T>
  requires Comparable<T> && Printable<T>
class DynamicArray {
  size_t capacity = 8;
  size_t size;
  T *arr;

  const float MULTIPLIER = 1.5;

  /// @brief Allocate new array with current `capacity`, copy data from `arr` to
  /// it and set the new array to `arr`
  void realloc() {
    T *new_arr = new T[capacity];

    std::memcpy(new_arr, arr, sizeof(T) * size);

    delete[] arr;
    arr = new_arr;
  }

public:
  DynamicArray() : arr(new T[capacity]), size(0) {}

  DynamicArray(const DynamicArray &) = delete;
  DynamicArray &operator=(const DynamicArray &) = delete;

  ~DynamicArray() { delete[] arr; }

  /// @brief Add new element to the table
  /// @param Data Variable you want to push
  void push_back(T data) {
    if (size + 1 > capacity) {
      capacity *= MULTIPLIER;
      realloc();
    }

    arr[size++] = data;
  }

  /// @brief Reserve given amount of memory
  /// @param amount Number of elements you will probably need
  ///
  /// This may be useful, if you know you'll need a lot of memory and don't want
  /// to risk to many reallocations
  void reserve(size_t amount) {
    assert(size > capacity &&
           "New capacity cannot be smaller than current size");

    capacity = amount;
    realloc();
  }

  T &operator[](size_t index) { return arr[index]; }
  const T &operator[](size_t index) const { return arr[index]; }

  /// @brief String representation of the array
  std::string as_string() {
    std::stringstream s;
    s << '[';

    for (size_t i = 0; i < size; i++) {
      s << arr[i];

      if (i < size - 1) {
        s << ',';
      }
    }
    s << ']';

    return s.str();
  }

  /// @brief String representation of the array in shortened form
  std::string as_string_summary() {
    std::stringstream s;
    s << '{';
    s << "Size: " << size << ", Capacity: " << capacity;
    s << " | ";
    if (size < 6) {
      s << as_string();
      return s.str();
    }

    s << '[';
    for (size_t i = 0; i < 3; i++) {
      s << arr[i];

      if (i < 2) {
        s << ',';
      }
    }

    s << ", ... ,";

    for (size_t i = size - 1; i > size - 3; i--) {
      s << arr[i];

      if (i > size - 2) {
        s << ',';
      }
    }
    s << ']';

    s << '}';
    return s.str();
  }

  /// @brief Clear the array
  void clear() {
    capacity = 8;
    delete[] arr;
    arr = new T[capacity];
    size = 0;
  }

  size_t get_size() const { return size; }
  size_t get_capacity() const { return capacity; }
};
