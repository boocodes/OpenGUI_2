#pragma once
#include <glm/glm.hpp>
#include "../types/types.h"
#include <glm/gtc/matrix_transform.hpp>
extern int WIDTH;
extern int HEIGHT;

extern glm::mat4 pixel_simple_placement_projection;

namespace Global {
    extern  Shader font_shader;
    extern Shader img_shader;
    extern Shader pixel_placement_shader;
    void init_shaders();
}
