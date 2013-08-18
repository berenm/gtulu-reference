#include "gtulu/notimp.hpp"

#include "gtulu/logging.hpp"

#include <cstdlib>
#include <stdexcept>

#include <corefungi.hpp>

namespace gtulu {
  namespace api {

    void __not_implemented(char const* const name) {
      __warn() << "not implemented: " << name;

      static bool const __gtulu_ni_throw = corefungi::get("gtulu.notimp-throw");
      if (__gtulu_ni_throw)
        throw std::runtime_error(std::string("not implemented: ") + name);
    }

  }
}
