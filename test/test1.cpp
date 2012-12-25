#include <gtulu/api.hpp>

int main(int argc, char const* argv[]) {
  gtulu::api::active_texture(0);
  gtulu::api::attach_shader(1, 2);

  gtulu::api::uniform(0, {
                        gtulu::vec3< uint32_t > { 1, 2, 3 },
                        gtulu::vec3< uint32_t > { 2, 3, 4 },
                        gtulu::vec3< uint32_t > { 3, 4, 5 }
                      });

  return 0;
}
