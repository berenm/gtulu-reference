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

    template< typename T, typename ... Ts >
    struct is_one_of;

    template< typename T >
    struct is_one_of< T > {
      static constexpr bool value = false;
    };

    template< typename T, typename T0, typename ... Ts >
    struct is_one_of< T, T0, Ts ... > {
      static constexpr bool value = std::is_same< T, T0 >::value || is_one_of< T, Ts ... >::value;
    };

  }

  template< typename T, typename ... Ts >
  struct is_one_of {
    static constexpr bool value = detail::is_one_of< T, Ts ... >::value;
    static_assert(value, "T is not one of the allowed constants.");
  };

}

#endif // ifndef __GTULU_TYPES_TAGGED_VALUE_HPP__
