#include <cassert>
#include <cstddef>
#include <cstring>
#include <custom_concepts.hpp>
#include <sstream>

static const size_t INITIAL_CAPACITY = 8;

template <typename T>
  requires Comparable<T> && Printable<T>
class DynamicArray {
  size_t capacity = INITIAL_CAPACITY;
  size_t size;
  T *arr;

  const float MULTIPLIER = 1.5;

  /// @brief Allocate new array with current `capacity`, copy data from `arr` to
  /// it and set the new array to `arr`
  void realloc() {
    T *new_arr = new T[capacity];

    for (size_t i = 0; i < size; i++) {
      new_arr[i] = std::move(arr[i]);
    }

    delete[] arr;
    arr = new_arr;
  }

  static const auto &unwrap(T &data) {
    if constexpr (std::is_pointer_v<T>) {
      return *data;
    } else {
      return data;
    }
  }

public:
  DynamicArray() : arr(new T[capacity]), size(0) {}

  DynamicArray(const DynamicArray &) = delete;
  DynamicArray &operator=(const DynamicArray &) = delete;

  ~DynamicArray() { delete[] arr; }

  /// @brief Add new element to the table
  /// @param Data Variable you want to push
  /// @return True if push warranted a reallocation
  bool push_back(T data) {
    bool is_realloc = false;
    if (size + 1 > capacity) {
      capacity *= MULTIPLIER;
      realloc();
      is_realloc = true;
    }

    arr[size++] = data;
    return is_realloc;
  }

  /// @brief Reserve given amount of memory
  /// @param amount Number of elements you will probably need
  ///
  /// This may be useful, if you know you'll need a lot of memory and don't want
  /// to risk to many reallocations
  void reserve(size_t amount) {
    assert(amount > capacity &&
           "New capacity cannot be smaller than current size");

    capacity = amount;
    realloc();
  }

  T &operator[](size_t index) {
    assert(index < size && "Index out of bounds");
    return arr[index];
  }

  const T &operator[](size_t index) const {
    assert(index < size && "Index out of bounds");
    return arr[index];
  }

  /// @brief String representation of the array
  std::string as_string() {
    std::stringstream s;
    s << '[';

    for (size_t i = 0; i < size; i++) {
      s << unwrap(arr[i]);

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
      s << '}';
      return s.str();
    }

    s << '[';
    for (size_t i = 0; i < 3; i++) {
      s << unwrap(arr[i]);

      if (i < 2) {
        s << ',';
      }
    }

    s << ", ... ,";

    for (size_t i = size - 3; i < size; i++) {
      s << unwrap(arr[i]);

      if (i < size - 1) {
        s << ',';
      }
    }
    s << ']';

    s << '}';
    return s.str();
  }

  /// @brief Clear the array
  /// @param should_free_members Is set to true and `T` is a pointer, it will
  /// deallocate data it holds.
  void clear(bool should_free_members = false) {
    if constexpr (std::is_pointer_v<T>) {
      if (should_free_members) {
        for (size_t i = 0; i < size; i++) {
          delete arr[i];
        }
      }
    }

    capacity = INITIAL_CAPACITY;
    delete[] arr;
    arr = new T[capacity];
    size = 0;
  }

  /// @brief Sort data in descending order (from biggest to smallest).
  /// O(n^2) complexity, where n is current array size
  void bubble_sort() {
    if (size == 0) {
      return;
    }

    for (size_t i = 0; i < size; i++) {
      for (size_t j = 0; j < size - i - 1; j++) {
        if (unwrap(arr[j]) < unwrap(arr[j + 1])) {
          std::swap(arr[j], arr[j + 1]);
        }
      }
    }
  }

  size_t get_size() const { return size; }
  size_t get_capacity() const { return capacity; }
};
