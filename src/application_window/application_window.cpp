#include "application_window.h"

static GLFWwindow* window;



static void processInput(GLFWwindow* window, int button, int action, int mods);
static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	WIDTH = width;
	HEIGHT = height;
	Global::compute_projections();
}

void show_application_window()
{
	WIDTH = 1380;
	HEIGHT = 920;
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_DECORATED, false);
	window = glfwCreateWindow(1380, 920, "OpenGUI", NULL, NULL);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	int max_width = GetSystemMetrics(SM_CXSCREEN);
	int max_hieght = GetSystemMetrics(SM_CYSCREEN);
	glfwSetWindowMonitor(window, NULL, (max_width / 2) - (1380 / 2), (max_hieght / 2) - (920 / 2), 1380, 920, GLFW_DONT_CARE);
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
	UI_ELEM_LIST root_ui;
	root_ui.add_elem(std::make_unique<UI_FONT>(15, 898, glm::vec3(0.5294117647058824, 0.5294117647058824, 0.5294117647058824), 12, "assets/font/Roboto-Regular.ttf", "OpenGUI", 2));
	root_ui.add_elem(std::make_unique<UI_DIV>(WIDTH, 1, 0, 887, 4, glm::vec3(0.9137254901960784, 0.9137254901960784, 0.9137254901960784)));
	root_ui.add_elem(std::make_unique<UI_IMG>(13, 13, 83, 910, "assets/hamburger_menu.png", 1));
	root_ui.add_elem(std::make_unique<UI_IMG>(9, 9, 183, 908, "assets/arrow_down.png", 1));
	root_ui.add_elem(std::make_unique<UI_IMG>(17, 17, 1175, 912, "assets/build_icon.png", 1));
	root_ui.add_elem(std::make_unique<UI_IMG>(14, 14, 1211, 910, "assets/search_icon.png", 1));
	root_ui.add_elem(std::make_unique<UI_IMG>(14, 14, 1249, 910, "assets/settings_icon.png", 1));
	root_ui.add_elem(std::make_unique<UI_DIV>(1, 24, 1284, 916, 4, glm::vec3(0.9137254901960784, 0.9137254901960784, 0.9137254901960784)));
	root_ui.add_elem(std::make_unique<UI_FONT>(100, 899, glm::vec3(0.2784313725490196, 0.2784313725490196, 0.2784313725490196), 12, "assets/font/Roboto-Regular.ttf", project_name, 2));
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	while (!glfwWindowShouldClose(window))
	{
		glfwSetMouseButtonCallback(window, processInput);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		root_ui.render();
		
		glfwSwapBuffers(window);
		glfwPollEvents();

	}
	glfwTerminate();
	return;
}


void processInput(GLFWwindow* window, int button, int action, int mods)
{
	double x, y;
	glfwGetCursorPos(window, &x, &y);
	// hover events

	mouse_x_pos = static_cast<int>(x);
	mouse_y_pos = static_cast<int>(y);
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (action == GLFW_PRESS) {
			
		}
		else if (action == GLFW_RELEASE) {
			// Обработка отпускания

		}
	}
}
