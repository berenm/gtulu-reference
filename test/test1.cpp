#include <gtulu/api.hpp>

int main(int argc, char const* argv[]) {
  gtulu::api::active_texture(0);
  gtulu::api::attach_shader(1, 2);

  std::vector< gtulu::vec3< float > > const uniform_data = { {{ 1, 2, 3 }}, {{ 2, 3, 4 }}, {{ 3, 4, 5 }} };
  gtulu::api::uniform(0, uniform_data);

  gtulu::api::compressed_tex_image(gtulu::cst::texture_1d, 0, gtulu::cst::rgb8, gtulu::vec1< gtulu::size > {{ 16 }}, 0, 128, gtulu::buffer_ref(0));

  return 0;
}
