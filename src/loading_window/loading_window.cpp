#include "loading_window.h"






void progress_bar()
{
		
}


UI_ELEM_LIST loading_ui_list;
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	WIDTH = width;
	HEIGHT = height;
	Global::compute_projections();
}

void init_ui()
{
	UI_FONT opengui_title(64, 480, glm::vec3(1, 1, 1), 0.8f, "assets/font/Roboto-Bold.ttf", "OpenGUI", 0.2f);
	UI_IMG main_img(836, 600, 0, 600, "assets/loader_background.jpg", 0.0f);
	UI_DIV loader_full_line(WIDTH, 4, 0, 21, 0.4f, glm::vec3(0.8745098039215686, 0.8745098039215686, 0.8745098039215686));
	UI_FONT version_text(64, 450, glm::vec3(1, 1, 1), 0.35f, "assets/font/Roboto-Regular.ttf", "2025.0.1", 0.2f);
	UI_DIV loading_elem(200, 50, 100, 200, 0.4f, glm::vec3(0, 1, 0));
	loading_elem.centred();
	loading_ui_list.add_ui_img(main_img);
	loading_ui_list.add_ui_font(version_text);
	loading_ui_list.add_ui_font(opengui_title);
	loading_ui_list.add_ui_div(loading_elem);
	loading_ui_list.add_ui_div(loader_full_line);
}


void show_loading_window()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_DECORATED, false);
	GLFWwindow* window = glfwCreateWindow(836, 600, "OpenGUI", NULL, NULL);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	int max_width = GetSystemMetrics(SM_CXSCREEN);
	int max_hieght = GetSystemMetrics(SM_CYSCREEN);
	glfwSetWindowMonitor(window, NULL, (max_width / 2) - (836 / 2), (max_hieght / 2) - (600 / 2), 836, 600, GLFW_DONT_CARE);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return;
	}

	Global::init_shaders();
	Global::compute_projections();
	init_ui();
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
    while (!glfwWindowShouldClose(window))
    {
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		loading_ui_list.render();
        glfwSwapBuffers(window);
        glfwPollEvents();
		
    }
    glfwTerminate();
    return;
}


