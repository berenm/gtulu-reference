#ifndef __GTULU_TYPES_TAGGED_VALUE_HPP__
#define __GTULU_TYPES_TAGGED_VALUE_HPP__

#include <cstdint>

namespace gtulu {
  namespace gtu = ::gtulu;

  template< typename Tag, typename ValueType = uint32_t >
  struct tagged_value {
    typedef Tag       tag;
    typedef ValueType value_type;

    tagged_value() = default;
    tagged_value(ValueType const value) : value(value) {}

    operator ValueType() const { return value; }
    operator ValueType&() { return value; }
    ValueType const* operator&() const { return &value; }
    ValueType* operator&() { return &value; }

    protected:
      ValueType value;
  };

  template< typename Tag, typename ValueType = uint32_t, typename... Tags >
  struct tagged_value_convertible;

  template< typename Tag, typename ValueType >
  struct tagged_value_convertible< Tag, ValueType > : gtu::tagged_value< Tag, ValueType > {};

  template< typename Tag, typename ValueType, typename T, typename... Tags >
  struct tagged_value_convertible< Tag, ValueType, T, Tags... > : tagged_value_convertible< Tag, ValueType, Tags... > {
    using target_type = gtu::tagged_value< T, ValueType >;
    operator target_type() const { return target_type(this->value); }
  };

}

#endif // ifndef __GTULU_TYPES_TAGGED_VALUE_HPP__
