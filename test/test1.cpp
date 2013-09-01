#include <gtulu/api.hpp>

#include <EGL/egl.h>
#include <EGL/eglext.h>

#include <boost/thread.hpp>
#include <corefungi.hpp>

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
  EGLDisplay display;
  EGLContext context;
  EGLSurface drawable;
  EGLSurface readable;
  bool       status;

  egl_context_impl(EGLDisplay const display, EGLContext const context, EGLSurface const drawable, EGLSurface const readable) :
    display(display),
    context(context),
    drawable(drawable),
    readable(readable) {}

  bool acquire() {
    return eglMakeCurrent(display, drawable, readable, context);
  }

  void release() {
    eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
  }

};

struct egl_context : context_info_base< egl_context_impl > {
  egl_context(EGLDisplay const display=nullptr, EGLContext const context=EGL_NO_CONTEXT, EGLSurface const drawable=EGL_NO_SURFACE, EGLSurface const readable=EGL_NO_SURFACE) :
    context_info_base< egl_context_impl >(display, context, drawable, readable) {}
};

struct egl_current_context : public egl_context {
  egl_current_context() :
    egl_context(eglGetCurrentDisplay(), eglGetCurrentContext(), eglGetCurrentSurface(EGL_DRAW), eglGetCurrentSurface(EGL_READ)) {}
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

  EGLNativeDisplayType const native_display = XOpenDisplay(nullptr);
  EGLDisplay const           display        = eglGetDisplay(native_display);

  if (display == nullptr) {
    __fatal() << "Unable to open X display.";
    throw std::runtime_error("Unable to open X display.");
  }

  eglInitialize(display, nullptr, nullptr);

  __info() << "EGL client-apis: " << eglQueryString(display, EGL_CLIENT_APIS);
  __info() << "EGL extensions: " << eglQueryString(display, EGL_EXTENSIONS);
  __info() << "EGL vendor: " << eglQueryString(display, EGL_VENDOR);
  __info() << "EGL version: " << eglQueryString(display, EGL_VERSION);

  EGLint       config_count;
  EGLint const attributes[] = {
    EGL_CONFIG_CAVEAT,   EGL_NONE,
    EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
    EGL_CONFORMANT,      EGL_OPENGL_BIT,
    EGL_SURFACE_TYPE,    EGL_WINDOW_BIT | EGL_PBUFFER_BIT | EGL_PIXMAP_BIT,
    EGL_NONE
  };

  if (eglChooseConfig(display, attributes, nullptr, 0, &config_count) != EGL_TRUE) {
    __fatal() << "Unable to retrieve framebuffer configurations count.";
    throw std::runtime_error("Unable to retrieve framebuffer configurations count.");
  }

  EGLConfig* const configs = new EGLConfig[config_count];
  if (eglChooseConfig(display, attributes, configs, config_count, &config_count) != EGL_TRUE) {
    __fatal() << "Unable to choose framebuffer configuration.";
    throw std::runtime_error("Unable to choose framebuffer configuration.");
  }

  eglBindAPI(EGL_OPENGL_API);
  EGLint const     context_attributes[] = {
    // EGL_CONTEXT_FLAGS_KHR, EGL_CONTEXT_OPENGL_FORWARD_COMPATIBLE_BIT_KHR | EGL_CONTEXT_OPENGL_DEBUG_BIT_KHR,
    // EGL_CONTEXT_CLIENT_VERSION ,       3,
    // EGL_CONTEXT_MINOR_VERSION_KHR,       3,
    // EGL_CONTEXT_OPENGL_PROFILE_MASK_KHR, EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT_KHR,
    EGL_NONE
  };
  EGLContext const context = eglCreateContext(display, configs[0], EGL_NO_CONTEXT, context_attributes);
  if (context == EGL_NO_CONTEXT) {
    __fatal() << "Unable to create OpenGL context.";
    throw std::runtime_error("Unable to create OpenGL context.");
  }

  // XSync(native_display, false);

  context_info context_info(display, context, eglCreatePbufferSurface(display, configs[0], nullptr), eglCreatePbufferSurface(display, configs[0], nullptr));
  if (!context_info.try_acquire()) {
    __fatal() << "Unable to attach context to default drawable.";
    throw std::runtime_error("Unable to attach context to default drawable.");
  }

  delete[] configs;
} // _create

static void destroy_context() {
  context_info context_info = current_context_info();

  context_info.acquire();
  context_info.release();

  eglDestroyContext(context_info.display, context_info.context);
  eglDestroySurface(context_info.display, context_info.drawable);
  eglDestroySurface(context_info.display, context_info.readable);
  eglTerminate(context_info.display);

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

  std::vector< gtulu::vec3< float > > const uniform_data = { {{ 1, 2, 3 }}, {{ 2, 3, 4 }}, {{ 3, 4, 5 }} };
  gtulu::api::uniform(0, uniform_data);

  gtulu::api::compressed_tex_image(gtulu::cst::texture_1d, 0, gtulu::cst::rgb8, gtulu::vec1< gtulu::size > {{ 16 }}, 0, 128, gtulu::buffer_ref(0));

  gtulu::vertex_double vdouble1 = 1;
  gtulu::vertex_double vdouble2 = 1.0f;
  gtulu::vertex_double vdouble3 = 1.0;
  gtulu::vertex_attrib attrib1 = 1;
  gtulu::vertex_attrib attrib2 = 1.0f;
  gtulu::vertex_attrib attrib3 = 1.0;
  gtulu::vertex_attrib attrib4 = vdouble2;
  gtulu::vertex_float float1 = 1;
  gtulu::vertex_float float2 = 1.0f;
  gtulu::vertex_float float3 = 1.0;
  // gtulu::vertex_float float4 = vdouble1; // compile error as expected
  gtulu::vertex_float float5 = attrib1;
  gtulu::vertex_integer integer1 = 1;
  gtulu::vertex_integer integer2 = 1.0f;
  gtulu::vertex_integer integer3 = 1.0;
  // gtulu::vertex_integer integer4 = vdouble1; // compile error as expected
  gtulu::vertex_integer integer5 = attrib1;

  // gtulu::api::vertex_attrib(vdouble1, gtulu::vec3< int >{1,2,3}); // compile error as expected
  // gtulu::api::vertex_attrib(vdouble2, gtulu::vec3< float >{1.0f,2.0f,3.0f}); // compile error as expected
  gtulu::api::vertex_attrib(vdouble3, gtulu::vec3< double >{1.0,2.0,3.0});
  // gtulu::api::vertex_attrib(attrib1,  gtulu::vec4< int32_t >{1,2,3,4}); // ambiguous
  gtulu::api::vertex_attrib(float1,   gtulu::vec3< int16_t >{1,2,3});
  gtulu::api::vertex_attrib(float2,   gtulu::vec3< float >{1.0f,2.0f,3.0f});
  gtulu::api::vertex_attrib(float3,   gtulu::vec3< double >{1.0,2.0,3.0});
  gtulu::api::vertex_attrib(integer1, gtulu::vec3< int32_t >{1,2,3});
  // gtulu::api::vertex_attrib(integer2, gtulu::vec3< float >{1.0f,2.0f,3.0f}); // compile error as expected
  // gtulu::api::vertex_attrib(integer3, gtulu::vec3< double >{1.0,2.0,3.0}); // compile error as expected

  destroy_context();
  return 0;
}
