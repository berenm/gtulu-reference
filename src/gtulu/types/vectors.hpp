#ifndef __GTULU_TYPES_VECTORS_HPP__
#define __GTULU_TYPES_VECTORS_HPP__

#include <array>

namespace gtulu {
  namespace gtu = ::gtulu;

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
}

#endif // ifndef __GTULU_TYPES_VECTORS_HPP__
