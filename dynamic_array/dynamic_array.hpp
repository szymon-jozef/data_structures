#include <cstddef>
#include <cstring>
#include <sstream>

template <typename T> class DynamicArray {
  size_t capacity = 8;
  size_t size;
  T *arr;

  float multiplier;

  void realloc() {
    capacity *= multiplier;

    T *new_arr = new T[capacity];

    std::memcpy(new_arr, arr, sizeof(T) * size);

    arr = new_arr;
  }

public:
  DynamicArray()
      : arr(new T[capacity]), capacity(8), size(0), multiplier(2.5) {}

  /// Add new element to the table
  /// @param Data Variable you want to push
  void push_back(T data) {
    if (size + 1 > capacity) {
      realloc();
    }

    arr[size++] = data;
  }

  T &operator[](size_t index) { return arr[index]; }
  const T &operator[](size_t index) const { return arr[index]; }

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

  size_t get_size() const { return size; }
};
