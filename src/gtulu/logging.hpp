#ifndef __GTULU_API_LOGGING_HPP__
#define __GTULU_API_LOGGING_HPP__

#include <logging/logging.hpp>
#include <gtulu/ostream.hpp>

namespace gtulu {
  namespace api {

    namespace logging {
      struct context {
        static std::string const module;
      };

      typedef::logging::logger_maker< context > log;
    }

  }
}

#endif // ifndef __GTULU_API_LOGGING_HPP__
