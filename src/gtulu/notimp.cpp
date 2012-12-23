#include "gtulu/notimp.hpp"

#include "gtulu/logging.hpp"

#include <cstdlib>
#include <stdexcept>

namespace gtulu {
  namespace api {

    void __not_implemented(char const* const name) {
      __warn() << "not implemented: " << name;

      static char const* const __gtulu_ni_throw = std::getenv("GTULU_NOTIMP_THROW");
      if (__gtulu_ni_throw != nullptr)
        throw std::runtime_error(std::string("not implemented: ") + name);
    }

  }
}
