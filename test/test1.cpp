#include <gtulu/api.hpp>

int main(int argc, char const* argv[]) {
  gtulu::api::active_texture(0);
  gtulu::api::attach_shader(1, 2);

  std::vector< gtulu::vec3< uint32_t > > const uniform_data = { {{ 1, 2, 3 }}, {{ 2, 3, 4 }}, {{ 3, 4, 5 }} };
  gtulu::api::uniform(0, uniform_data);

  return 0;
}
