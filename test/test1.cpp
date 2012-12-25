#include <gtulu/api.hpp>

int main(int argc, char const* argv[]) {
  gtulu::api::active_texture(0);
  gtulu::api::attach_shader(1, 2);

  return 0;
}
