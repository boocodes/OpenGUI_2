#include "loading_window.h"


static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	WIDTH = width;
	HEIGHT = height;
	Global::compute_projections();
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
	
	int progress = 0;
	UI_ELEM_LIST root_ui;
	root_ui.add_elem(std::make_unique<UI_IMG>(WIDTH, HEIGHT, 0, HEIGHT, "assets/loader_background.jpg", 1));
	root_ui.add_elem(std::make_unique<UI_FONT>(337, 280, glm::vec3(1, 1, 1), 40, "assets/font/Roboto-Bold.ttf", "OpenGUI", 2));
	root_ui.add_elem(std::make_unique<UI_FONT>(390, 266, glm::vec3(1, 1, 1), 15, "assets/font/Roboto-Medium.ttf", "2025.0.1", 2));
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
    while (!glfwWindowShouldClose(window))
    {
		if (progress >= WIDTH)
		{
			std::cout << progress << std::endl;
			glfwTerminate();
			return;
		}
		progress += 50;
		Sleep(100);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		root_ui.render();
        glfwSwapBuffers(window);
        glfwPollEvents();
		
    }
   
    return;
}


