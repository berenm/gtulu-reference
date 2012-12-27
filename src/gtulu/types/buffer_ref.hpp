#ifndef __GTULU_TYPES_BUFFER_REF_HPP__
#define __GTULU_TYPES_BUFFER_REF_HPP__

#include <gtulu/types/objects.hpp>
#include <gtulu/types/literals.hpp>

#include <cstdint>
#include <ostream>

namespace gtulu {
  namespace gtu = ::gtulu;

  struct buffer_ref {
    buffer_ref() = default;

    template< typename T >
    buffer_ref(std::vector< T >& data) :
      is_offset_(false),
      size_(data.size()),
      data_(&data[0])
    {}

    buffer_ref(gtu::size const offset, gtu::size const size=0) :
      is_offset_(true),
      size_(size),
      data_(static_cast< void* >(static_cast< uint8_t* >(nullptr) + offset))
    {}

    bool        is_offset() const { return this->is_offset_; }
    void*       data() { return this->data_; }
    void const* data() const { return this->data_; }
    gtu::size   size() const { return this->size_; }

    operator void*() { return this->data(); }
    operator void const*() const { return this->data(); }

    private:
      bool      is_offset_;
      gtu::size size_;
      void*     data_;
  };

  static inline std::ostream& operator<<(std::ostream& s, buffer_ref const& b) {
    if (b.is_offset())
      s << "{offset:" << (static_cast< uint8_t const* >(b.data()) - static_cast< uint8_t const* >(nullptr))
        << ", size:" << b.size()
        << "}";
    else
      s << "{data: " << b.data()
        << ", size:" << b.size()
        << "}";

    return s;
  }

} // namespace gtulu

#endif // ifndef __GTULU_TYPES_BUFFER_REF_HPP__
