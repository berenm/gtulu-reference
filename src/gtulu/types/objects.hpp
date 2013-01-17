#ifndef __GTULU_TYPES_OBJECTS_HPP__
#define __GTULU_TYPES_OBJECTS_HPP__

#include <gtulu/types/tagged_value.hpp>

namespace gtulu {
  namespace gtu = ::gtulu;

  namespace tag {
    struct constant;

    struct buffer;
    struct framebuffer;
    struct list;
    struct program;
    struct pipeline;
    struct query;
    struct renderbuffer;
    struct sampler;
    struct shader;
    struct texture;
    struct transform_feedback;
    struct vertex_array;
    struct vertex_attrib;
    struct uniform;
    struct uniform_block;
    struct subroutine;
    struct frag_data;
    struct frag_data_index;
    struct program_resource;
    struct debug_message;

    struct texture_unit;
  }

  typedef gtu::tagged_value< gtu::tag::constant, uint32_t > constant;
  typedef gtu::constant                                     checked_constant;

  typedef gtu::tagged_value< gtu::tag::buffer >             buffer;
  typedef gtu::tagged_value< gtu::tag::framebuffer >        framebuffer;
  typedef gtu::tagged_value< gtu::tag::list >               list;
  typedef gtu::tagged_value< gtu::tag::program >            program;
  typedef gtu::tagged_value< gtu::tag::pipeline >           pipeline;
  typedef gtu::tagged_value< gtu::tag::query >              query;
  typedef gtu::tagged_value< gtu::tag::renderbuffer >       renderbuffer;
  typedef gtu::tagged_value< gtu::tag::sampler >            sampler;
  typedef gtu::tagged_value< gtu::tag::shader >             shader;
  typedef gtu::tagged_value< gtu::tag::texture >            texture;
  typedef gtu::tagged_value< gtu::tag::transform_feedback > transform_feedback;
  typedef gtu::tagged_value< gtu::tag::vertex_array >       vertex_array;
  typedef gtu::tagged_value< gtu::tag::vertex_attrib >      vertex_attrib;
  typedef gtu::tagged_value< gtu::tag::uniform >            uniform;
  typedef gtu::tagged_value< gtu::tag::uniform_block >      uniform_block;
  typedef gtu::tagged_value< gtu::tag::subroutine >         subroutine;
  typedef gtu::tagged_value< gtu::tag::frag_data >          frag_data;
  typedef gtu::tagged_value< gtu::tag::frag_data_index >    frag_data_index;
  typedef gtu::tagged_value< gtu::tag::program_resource >   program_resource;
  typedef gtu::tagged_value< gtu::tag::debug_message >      debug_message;

  typedef gtu::tagged_value< gtu::tag::texture_unit > texture_unit;

}

#endif // ifndef __GTULU_TYPES_OBJECTS_HPP__
