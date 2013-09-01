#ifndef __GTULU_TYPES_OBJECTS_HPP__
#define __GTULU_TYPES_OBJECTS_HPP__

#include <gtulu/types/tagged_value.hpp>

namespace gtulu {
  namespace gtu = ::gtulu;

  namespace tag {
    namespace gl {
      struct constant {};

      struct fixed_point {};

      struct buffer {};
      struct framebuffer {};
      struct list {};
      struct program {};
      struct pipeline {};
      struct query {};
      struct renderbuffer {};
      struct sampler {};
      struct shader {};
      struct texture {};
      struct transform_feedback {};
      struct vertex_array {};
      struct uniform {};
      struct uniform_block {};
      struct subroutine {};
      struct frag_data {};
      struct frag_data_index {};
      struct program_resource {};
      struct debug_message {};

      struct vertex_attrib {};
      struct vertex_float :   vertex_attrib {};
      struct vertex_double :  vertex_attrib {};
      struct vertex_integer : vertex_attrib {};

      struct texture_unit {};
      struct vertex_binding {};
    }

    namespace egl {
      struct constant {};
      struct config {};
      struct context {};
      struct display {};
      struct surface {};
      struct client_buffer {};
    }
  }

  namespace egl {
    typedef gtu::tagged_value< gtu::tag::egl::constant, int32_t >    constant;
    typedef gtu::tagged_value< gtu::tag::egl::config, void* >        config;
    typedef gtu::tagged_value< gtu::tag::egl::context, void* >       context;
    typedef gtu::tagged_value< gtu::tag::egl::display, void* >       display;
    typedef gtu::tagged_value< gtu::tag::egl::surface, void* >       surface;
    typedef gtu::tagged_value< gtu::tag::egl::client_buffer, void* > client_buffer;

    typedef std::vector< egl::config > configs;

    struct version_info {
      int32_t major = 0;
      int32_t minor = 0;
    };

    typedef std::pair< gtu::egl::constant, int32_t > attribute;
    typedef std::vector< attribute >                 attributes;

  }

  namespace gl {
    typedef gtu::tagged_value< gtu::tag::gl::constant, uint32_t > constant;
    typedef gtu::gl::constant                                     checked_constant;

    typedef gtu::tagged_value< gtu::tag::gl::fixed_point, int8_t >   fixed8;
    typedef gtu::tagged_value< gtu::tag::gl::fixed_point, uint8_t >  ufixed8;
    typedef gtu::tagged_value< gtu::tag::gl::fixed_point, int16_t >  fixed16;
    typedef gtu::tagged_value< gtu::tag::gl::fixed_point, uint16_t > ufixed16;
    typedef gtu::tagged_value< gtu::tag::gl::fixed_point, int32_t >  fixed32;
    typedef gtu::tagged_value< gtu::tag::gl::fixed_point, uint32_t > ufixed32;
    typedef gtu::tagged_value< gtu::tag::gl::fixed_point, int64_t >  fixed64;
    typedef gtu::tagged_value< gtu::tag::gl::fixed_point, uint64_t > ufixed64;

    typedef gtu::tagged_value< gtu::tag::gl::buffer >             buffer;
    typedef gtu::tagged_value< gtu::tag::gl::framebuffer >        framebuffer;
    typedef gtu::tagged_value< gtu::tag::gl::list >               list;
    typedef gtu::tagged_value< gtu::tag::gl::program >            program;
    typedef gtu::tagged_value< gtu::tag::gl::pipeline >           pipeline;
    typedef gtu::tagged_value< gtu::tag::gl::query >              query;
    typedef gtu::tagged_value< gtu::tag::gl::renderbuffer >       renderbuffer;
    typedef gtu::tagged_value< gtu::tag::gl::sampler >            sampler;
    typedef gtu::tagged_value< gtu::tag::gl::shader >             shader;
    typedef gtu::tagged_value< gtu::tag::gl::texture >            texture;
    typedef gtu::tagged_value< gtu::tag::gl::transform_feedback > transform_feedback;
    typedef gtu::tagged_value< gtu::tag::gl::vertex_array >       vertex_array;
    typedef gtu::tagged_value< gtu::tag::gl::uniform >            uniform;
    typedef gtu::tagged_value< gtu::tag::gl::uniform_block >      uniform_block;
    typedef gtu::tagged_value< gtu::tag::gl::subroutine >         subroutine;
    typedef gtu::tagged_value< gtu::tag::gl::frag_data >          frag_data;
    typedef gtu::tagged_value< gtu::tag::gl::frag_data_index >    frag_data_index;
    typedef gtu::tagged_value< gtu::tag::gl::program_resource >   program_resource;
    typedef gtu::tagged_value< gtu::tag::gl::debug_message >      debug_message;

    typedef gtu::tagged_value< gtu::tag::gl::vertex_attrib >  vertex_attrib;
    typedef gtu::tagged_value< gtu::tag::gl::vertex_float >   vertex_float;
    typedef gtu::tagged_value< gtu::tag::gl::vertex_double >  vertex_double;
    typedef gtu::tagged_value< gtu::tag::gl::vertex_integer > vertex_integer;

    typedef gtu::tagged_value< gtu::tag::gl::texture_unit >   texture_unit;
    typedef gtu::tagged_value< gtu::tag::gl::vertex_binding > vertex_binding;
  }

  using namespace gl;
}

#endif // ifndef __GTULU_TYPES_OBJECTS_HPP__
