#include "gtulu/error.hpp"

#include "gtulu/api.hpp"
#include "gtulu/logging.hpp"

#include <cstdlib>
#include <stdexcept>

namespace gtulu {
  namespace api {

    // taken from the glcorearb.h
#define GL_NO_ERROR                      0
#define GL_INVALID_ENUM                  0x0500
#define GL_INVALID_VALUE                 0x0501
#define GL_INVALID_OPERATION             0x0502
#define GL_STACK_OVERFLOW                0x0503
#define GL_STACK_UNDERFLOW               0x0504
#define GL_OUT_OF_MEMORY                 0x0505
#define GL_INVALID_FRAMEBUFFER_OPERATION 0x0506

    void __check_error() {
      static char const* const __gtulu_error_nocheck = std::getenv("GTULU_ERROR_NOCHECK");

      if (__gtulu_error_nocheck != nullptr)
        return;

      gtu::cst::error const __gl_error        = gtulu::api::get_error();
      char const*           __gl_error_string = nullptr;
      switch (__gl_error) {
        case gtu::cst::error::no_error:
          return;

        case gtu::cst::error::invalid_enum:
          __gl_error_string = "invalid_enum raised: An unacceptable value is specified for an enumerated argument.";
          break;

        case gtu::cst::error::invalid_value:
          __gl_error_string = "invalid_value raised: A numeric argument is out of range.";
          break;

        case gtu::cst::error::invalid_operation:
          __gl_error_string = "invalid_operator raised: The specified operation is not allowed in the current state.";
          break;

        case gtu::cst::error::stack_underflow:
          __gl_error_string = "stack_underflow raised: An attempt has been made to perform an operation that would cause an internal stack to underflow.";
          break;

        case gtu::cst::error::stack_overflow:
          __gl_error_string = "stack_overflow raised: An attempt has been made to perform an operation that would cause an internal stack to overflow.";
          break;

        case gtu::cst::error::out_of_memory:
          __gl_error_string = "out_of_memory raised: There is not enough memory left to execute the command.";
          break;

        case gtu::cst::error::invalid_framebuffer_operation:
          __gl_error_string = "invalid_framebuffer_operator raised: The framebuffer object is not complete.";
          break;

        default:
          __gl_error_string = "unknown error raised.";
          break;
      }

      __error() << __gl_error_string;

      static char const* const __gtulu_error_nothrow = std::getenv("GTULU_ERROR_NOTHROW");
      if (__gtulu_error_nothrow == nullptr)
        throw std::runtime_error(__gl_error_string);
    } // __check_error

  }
}
