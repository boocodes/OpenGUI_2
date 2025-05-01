#pragma once
#include <glad/glad.h>
#include <Windows.h>
#include "../utils//global/global.h"
#include "../utils/types/types.h"
#include <vector>
#include <GLFW/glfw3.h>



class Window_nav_group {
private:
	std::vector<UI_FONT> font_list;
	std::vector<UI_IMG> img_list;
	std::vector<UI_DIV> div_list;
public:
	Window_nav_group();
};