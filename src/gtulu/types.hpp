#ifndef __GTULU_TYPES_HPP__
#define __GTULU_TYPES_HPP__

#include <cstdint>
#include <bitset>
#include <array>
#include <vector>

#include <boost/config.hpp>

#ifdef __IN_GTULU_OPENGL_API__
# define GTULU_API_EXPORT __attribute__((visibility("default")))
# define GTULU_API_IMPORT BOOST_SYMBOL_IMPORT
#else /* ifdef __IN_GTULU_OPENGL_API__ */
# define GTULU_API_EXPORT BOOST_SYMBOL_IMPORT
# define GTULU_API_IMPORT __attribute__((visibility("default")))
#endif /* ifdef __IN_GTULU_OPENGL_API__ */

struct __GLsync;

namespace gtulu {
  namespace gtu = ::gtulu;

  typedef std::uint32_t     constant;
  typedef std::int32_t      size;
  typedef std::int16_t      float16;
  typedef float             float32;
  typedef double            float64;
  typedef std::bitset< 32 > bitmask;
  typedef void*             buffer_ref;
  typedef struct __GLsync*  barrier;

  template< class T > using vec1 = std::array< T, 1 >;
  template< class T > using vec2 = std::array< T, 2 >;
  template< class T > using vec3 = std::array< T, 3 >;
  template< class T > using vec4 = std::array< T, 4 >;

  template< class T > using mat2x2 = std::array< std::array< T, 2 >, 2 >;
  template< class T > using mat2x3 = std::array< std::array< T, 2 >, 3 >;
  template< class T > using mat2x4 = std::array< std::array< T, 2 >, 4 >;
  template< class T > using mat3x2 = std::array< std::array< T, 3 >, 2 >;
  template< class T > using mat3x3 = std::array< std::array< T, 3 >, 3 >;
  template< class T > using mat3x4 = std::array< std::array< T, 3 >, 4 >;
  template< class T > using mat4x2 = std::array< std::array< T, 4 >, 2 >;
  template< class T > using mat4x3 = std::array< std::array< T, 4 >, 3 >;
  template< class T > using mat4x4 = std::array< std::array< T, 4 >, 4 >;

} // namespace gtulu

#endif // ifndef __GTULU_TYPES_HPP__
