#include <gtulu/api.hpp>

int main(int argc, char const* argv[]) {
  gtulu::api::active_texture(0);
  gtulu::api::attach_shader(1, 2);

  gtulu::api::program_uniform_matrix_4x3(0, 1, 2, true, (double*) nullptr);

  return 0;
}
