#ifndef __GTULU_TYPES_TAGGED_VALUE_HPP__
#define __GTULU_TYPES_TAGGED_VALUE_HPP__

#include <cstdint>
#include "gtulu/logging.hpp"

namespace gtulu {
  namespace gtu = ::gtulu;

  template< typename Tag, typename T = uint32_t >
  struct tagged_value {
    typedef Tag tag;
    typedef T   value_type;

    constexpr tagged_value() = default;
    constexpr tagged_value(T const value) : value(value) {}

    constexpr operator T() const { return value; }

    // operator T&() { return value; }
    T const* operator&() const { return &value; }
    T* operator&() { return &value; }

    protected:
      T value;
  };

  namespace detail {

    template< typename T >
    constexpr bool is_one_of(T const value) { return false; }

    template< typename T, T v, T... Vs >
    constexpr bool is_one_of(T const value) { return value == v || is_one_of< T, Vs... >(value); }

  }

}

#endif // ifndef __GTULU_TYPES_TAGGED_VALUE_HPP__
