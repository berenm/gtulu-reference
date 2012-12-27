#ifndef __GTULU_TYPES_HPP__
#define __GTULU_TYPES_HPP__

#include <cstdint>
#include <bitset>
#include <array>
#include <vector>
#include <functional>
#include <type_traits>

#include <boost/config.hpp>

#ifdef __IN_GTULU_OPENGL_API__
# define GTULU_API_EXPORT __attribute__((visibility("default")))
# define GTULU_API_IMPORT BOOST_SYMBOL_IMPORT
#else /* ifdef __IN_GTULU_OPENGL_API__ */
# define GTULU_API_EXPORT BOOST_SYMBOL_IMPORT
# define GTULU_API_IMPORT __attribute__((visibility("default")))
#endif /* ifdef __IN_GTULU_OPENGL_API__ */

#include <gtulu/types/objects.hpp>
#include <gtulu/types/buffer_ref.hpp>
#include <gtulu/types/literals.hpp>
#include <gtulu/types/vectors.hpp>

struct __GLsync;

namespace gtulu {
  namespace gtu = ::gtulu;

  typedef std::bitset< 32 > bitmask;
  typedef struct __GLsync*  barrier;
  typedef void*             bytearray;

  typedef std::function< void (gtu::constant const source, gtu::constant const type, uint32_t const id,
                               gtu::constant const severity, gtu::size const length, char const* message, void* userParam)
                         > debug_callback;

} // namespace gtulu

#endif // ifndef __GTULU_TYPES_HPP__
