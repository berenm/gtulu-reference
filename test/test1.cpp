#include <gtulu/egl/1.4/api.hpp>
#include <gtulu/gl/4.3/comp/api.hpp>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <boost/thread.hpp>
#include <logging/logging.hpp>
#include <corefungi.hpp>

namespace egl = gtulu::egl;
namespace gl  = gtulu::gl;

template< typename ContextImpl >
struct context_info_base : ContextImpl {
  template< typename ... Ts > context_info_base(Ts ... vs) : ContextImpl(vs ...) {}

  void acquire() {
    while (!ContextImpl::acquire()) {
      boost::this_thread::sleep(boost::posix_time::milliseconds(1));

    }
  }

  bool try_acquire() {
    return ContextImpl::acquire();
  }

  void release() {
    ContextImpl::release();
  }

};

struct egl_context_impl {
  egl::display display;
  egl::context context;
  egl::surface drawable;
  egl::surface readable;
  bool         status;

  egl_context_impl(egl::display const display, egl::context const context, egl::surface const drawable, egl::surface const readable) :
    display(display),
    context(context),
    drawable(drawable),
    readable(readable) {}

  bool acquire() {
    return egl::api::make_current(display, drawable, readable, context);
  }

  void release() {
    egl::api::make_current(display, egl::cst::no_surface, egl::cst::no_surface, egl::cst::no_context);
  }

};

struct egl_context : context_info_base< egl_context_impl > {
  egl_context(egl::display const display=nullptr, egl::context const context=egl::cst::no_context, egl::surface const drawable=egl::cst::no_surface, egl::surface const readable=egl::cst::no_surface) :
    context_info_base< egl_context_impl >(display, context, drawable, readable) {}
};

struct egl_current_context : public egl_context {
  egl_current_context() :
    egl_context(egl::api::get_current_display(), egl::api::get_current_context(), egl::api::get_current_surface(egl::cst::draw), egl::api::get_current_surface(egl::cst::read)) {}
};

typedef egl_context         context_info;
typedef egl_current_context current_context_info;

static int _x_error(Display* display, XErrorEvent* error) {
  char error_string[1024];

  XGetErrorText(error->display, error->error_code, error_string, 1024);
  __error() << error_string;
  return 0;
}

static void create_context() {
  // setenv("EGL_DRIVER", "egl_glx", 0);
  XSetErrorHandler(_x_error);

  void* const        native_display = XOpenDisplay(nullptr);
  egl::display const display        = egl::api::get_display(native_display);

  if (display == nullptr) {
    __fatal() << "Unable to open X display.";
    throw std::runtime_error("Unable to open X display.");
  }

  auto const version = egl::api::initialize(display);

  __info() << "EGL version: " << version.major << "." << version.minor;
  __info() << "EGL client-apis: " << egl::api::query_string(display, egl::cst::client_apis);
  __info() << "EGL extensions: " << egl::api::query_string(display, egl::cst::extensions);
  __info() << "EGL vendor: " << egl::api::query_string(display, egl::cst::vendor);
  __info() << "EGL version: " << egl::api::query_string(display, egl::cst::version);

  egl::attributes const attributes = {
    egl::attribute { egl::cst::config_caveat,   egl::cst::none                                                      },
    egl::attribute { egl::cst::renderable_type, egl::cst::opengl_bit                                                },
    egl::attribute { egl::cst::conformant,      egl::cst::opengl_bit                                                },
    egl::attribute { egl::cst::surface_type,    egl::cst::window_bit | egl::cst::pbuffer_bit | egl::cst::pixmap_bit },
    egl::attribute { egl::cst::none,            egl::cst::none                                                      }
  };

  auto const& configs = egl::api::choose_config(display, attributes);

  egl::api::bind_api(egl::cst::opengl_api);
  egl::attributes const context_attributes = {
    // egl::cst::context_flags_khr,               egl::cst::context_opengl_forward_compatible_bit_khr | egl::cst::context_opengl_debug_bit_khr,
    // egl::cst::context_client_version,          3,
    // egl::cst::context_minor_version_khr,       3,
    // egl::cst::context_opengl_profile_mask_khr, egl::cst::context_opengl_core_profile_bit_khr,
    egl::attribute { egl::cst::none, egl::cst::none }
  };

  egl::context const context = egl::api::create_context(display, configs[0], egl::cst::no_context, context_attributes);

  // XSync(native_display, false);

  context_info context_info(display, context, egl::api::create_pbuffer_surface(display, configs[0], { egl::attribute { egl::cst::none, egl::cst::none } }), egl::api::create_pbuffer_surface(display, configs[0], { egl::attribute { egl::cst::none, egl::cst::none } }));
  if (!context_info.try_acquire()) {
    __fatal() << "Unable to attach context to default drawable.";
    throw std::runtime_error("Unable to attach context to default drawable.");
  }

} // _create

static void destroy_context() {
  context_info context_info = current_context_info();

  context_info.acquire();
  context_info.release();

  egl::api::destroy_context(context_info.display, context_info.context);
  egl::api::destroy_surface(context_info.display, context_info.drawable);
  egl::api::destroy_surface(context_info.display, context_info.readable);
  egl::api::terminate(context_info.display);

  // XCloseDisplay(context_info.display);
}

int main(int argc, char const* argv[]) {
  corefungi::init(argc, argv);
  create_context();

  __info() << "vendor: " << gtulu::api::get_string(gtulu::cst::vendor);
  __info() << "renderer: " << gtulu::api::get_string(gtulu::cst::renderer);
  __info() << "version: " << gtulu::api::get_string(gtulu::cst::version);
  __info() << "shading_language_version: " << gtulu::api::get_string(gtulu::cst::shading_language_version);

  std::vector< int32_t > count;
  gtulu::api::get(gtulu::cst::max_combined_texture_image_units, count);
  __debug() << "maximum combined textures: " << count;

  auto const& messages = gtulu::api::get_debug_message_log();
  for (auto const& message : messages) {
    std::clog << std::get< 4 >(message) << std::endl;
  }

  gtulu::api::active_texture(0);
  gtulu::api::attach_shader(1, 2);

  std::vector< gtulu::vec3< float > > const uniform_data = { { { 1, 2, 3 } }, { { 2, 3, 4 } }, { { 3, 4, 5 } } };
  gtulu::api::uniform(0, uniform_data);

  gtulu::api::compressed_tex_image(gtulu::cst::texture_1d, 0, gtulu::cst::rgb8, gtulu::vec1< gtulu::size > { { 16 } }, 0, 128, gtulu::buffer_ref(0));

  gtulu::vertex_double vdouble1 = 1;
  gtulu::vertex_double vdouble2 = 1.0f;
  gtulu::vertex_double vdouble3 = 1.0;
  gtulu::vertex_attrib attrib1  = 1;
  gtulu::vertex_attrib attrib2  = 1.0f;
  gtulu::vertex_attrib attrib3  = 1.0;
  gtulu::vertex_attrib attrib4  = vdouble2;
  gtulu::vertex_float  float1   = 1;
  gtulu::vertex_float  float2   = 1.0f;
  gtulu::vertex_float  float3   = 1.0;

  // gtulu::vertex_float float4 = vdouble1; // compile error as expected
  gtulu::vertex_float   float5   = attrib1;
  gtulu::vertex_integer integer1 = 1;
  gtulu::vertex_integer integer2 = 1.0f;
  gtulu::vertex_integer integer3 = 1.0;

  // gtulu::vertex_integer integer4 = vdouble1; // compile error as expected
  gtulu::vertex_integer integer5 = attrib1;

  // gtulu::api::vertex_attrib(vdouble1, gtulu::vec3< int >{1,2,3}); // compile error as expected
  // gtulu::api::vertex_attrib(vdouble2, gtulu::vec3< float >{1.0f,2.0f,3.0f}); // compile error as expected
  gtulu::api::vertex_attrib(vdouble3, gtulu::vec3< double > { 1.0, 2.0, 3.0 });

  // gtulu::api::vertex_attrib(attrib1,  gtulu::vec4< int32_t >{1,2,3,4}); // ambiguous
  gtulu::api::vertex_attrib(float1, gtulu::vec3< int16_t > { 1, 2, 3 });
  gtulu::api::vertex_attrib(float2, gtulu::vec3< float > { 1.0f, 2.0f, 3.0f });
  gtulu::api::vertex_attrib(float3, gtulu::vec3< double > { 1.0, 2.0, 3.0 });
  gtulu::api::vertex_attrib(integer1, gtulu::vec3< int32_t > { 1, 2, 3 });

  // gtulu::api::vertex_attrib(integer2, gtulu::vec3< float >{1.0f,2.0f,3.0f}); // compile error as expected
  // gtulu::api::vertex_attrib(integer3, gtulu::vec3< double >{1.0,2.0,3.0}); // compile error as expected

  destroy_context();
  return 0;
} // main
