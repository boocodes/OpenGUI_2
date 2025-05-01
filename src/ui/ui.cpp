#include "ui.h"



Window_nav_group::Window_nav_group()
{
	UI_DIV background(WIDTH, 38, 0, HEIGHT, 0.99f, glm::vec3(0.9647058823529412, 0.9647058823529412, 0.9647058823529412));
	UI_DIV underline_background(WIDTH, 1, 0, 645, 0.99f, glm::vec3(0.9137254901960784, 0.9137254901960784, 0.9137254901960784));

	UI_FONT app_version_text(22, 659, glm::vec3(0.34509803921568627, 0.34901960784313724, 0.3568627450980392), 0.3, "assets/font/Roboto-Regular.ttf", "OpenGUI v0.1", 0.999f);

	UI_DIV minimize_button(33, 38, 694, 659, 0.999f, glm::vec3(0, 0.49411764705882355, 1));
	UI_DIV close_button(33, 38, 729, 659, 0.999f, glm::vec3(0.7686274509803922, 0.16862745098039217, 0.10980392156862745));


}