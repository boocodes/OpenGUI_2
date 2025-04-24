#include "global.h"
#include <stdexcept>

int WIDTH = 836;
int HEIGHT = 600;


                                                

glm::mat4 pixel_simple_placement_projection = glm::ortho(0.0f, static_cast<float>(WIDTH), 0.0f, static_cast<float>(HEIGHT));
Shader Global::img_shader; // Определение глобального шейдера
Shader Global::font_shader;
void Global::init_shaders() {
    try {
        Global::img_shader.generate(
            "src/utils/shaders/img/img.vs",
            "src/utils/shaders/img/img.fs"
        );
        Global::font_shader.generate(
            "src/utils/shaders/font/font.vs",
            "src/utils/shaders/font/font.fs"
        );
    }
    catch (const std::exception& e) {
        // Обработка ошибок
    }
}