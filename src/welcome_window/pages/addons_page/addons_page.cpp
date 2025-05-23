#include "addons_page.h"



void show_addons_page()
{
	static UI_ELEM_LIST root_ui;
	static int title = root_ui.add_elem(std::make_unique<UI_FONT>(441, 341, glm::vec3(0, 0, 0), 16, "assets/font/Roboto-Regular.ttf", "In development", 11));

	root_ui.render();
}