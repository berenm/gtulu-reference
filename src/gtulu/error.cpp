#include "gtulu/error.hpp"

#include "gtulu/api.hpp"
#include "gtulu/logging.hpp"

#include <cstdlib>
#include <stdexcept>

#include <corefungi.hpp>

namespace gtulu {
  namespace api {

    static corefungi::sprout const o = {
      "Gtulu API options", {
        { "gtulu.no-error-check","disable the error checking after each GL call", corefungi::bool_switch },
        { "gtulu.no-error-throw","disable the exception throw in case of error", corefungi::bool_switch },
        { "gtulu.notimp-throw","enable the exception throw in case of not-implemented function call", corefungi::bool_switch }
      }
    };

    void __check_error() {
      static bool const __gtulu_error_nocheck = corefungi::get("gtulu.no-error-check");
      static bool const __gtulu_error_nothrow = corefungi::get("gtulu.no-error-throw");

      if (__gtulu_error_nocheck)
        return;

      gtu::constant const __gl_error        = gtulu::api::get_error();
      char const*         __gl_error_string = nullptr;
      switch (__gl_error) {
        case gtu::cst::no_error:

          return;

        case gtu::cst::invalid_enum:
          __gl_error_string = "invalid_enum raised: An unacceptable value is specified for an enumerated argument.";
          break;

        case gtu::cst::invalid_value:
          __gl_error_string = "invalid_value raised: A numeric argument is out of range.";
          break;

        case gtu::cst::invalid_operation:
          __gl_error_string = "invalid_operator raised: The specified operation is not allowed in the current state.";
          break;

#if GTULU_VERSION_COMPAT
        case gtu::cst::stack_underflow:
          __gl_error_string = "stack_underflow raised: An attempt has been made to perform an operation that would cause an internal stack to underflow.";
          break;

        case gtu::cst::stack_overflow:
          __gl_error_string = "stack_overflow raised: An attempt has been made to perform an operation that would cause an internal stack to overflow.";
          break;
#endif // if GTULU_VERSION_COMPAT

        case gtu::cst::out_of_memory:
          __gl_error_string = "out_of_memory raised: There is not enough memory left to execute the command.";
          break;

#if GTULU_VERSION_MAJOR > 2
        case gtu::cst::invalid_framebuffer_operation:
          __gl_error_string = "invalid_framebuffer_operator raised: The framebuffer object is not complete.";
          break;
#endif // if GTULU_VERSION_MAJOR > 2

        default:
          __gl_error_string = "unknown error raised.";
          break;
      }

      __error() << __gl_error_string;

      if (!__gtulu_error_nothrow)
        throw std::runtime_error(__gl_error_string);
    } // __check_error

  }
}
