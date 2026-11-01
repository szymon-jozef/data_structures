#pragma once

#include <concepts>
#include <ostream>

template <typename T>
concept Printable =
    requires(std::ostream &os, const std::remove_pointer_t<T> &t) {
      { os << t } -> std::same_as<std::ostream &>;
    };

template <typename T>
concept Comparable = std::totally_ordered<std::remove_pointer_t<T>>;
