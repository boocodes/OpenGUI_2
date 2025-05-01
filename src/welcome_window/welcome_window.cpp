#include "welcome_window.h"



GLFWwindow* window;

void processInput(GLFWwindow* window);






static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	WIDTH = width;
	HEIGHT = height;
	Global::compute_projections();
}

void show_welcome_window()
{
	WIDTH = 773;
	HEIGHT = 684;
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_DECORATED, false);
	window = glfwCreateWindow(773, 684, "OpenGUI", NULL, NULL);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	int max_width = GetSystemMetrics(SM_CXSCREEN);
	int max_hieght = GetSystemMetrics(SM_CYSCREEN);
	glfwSetWindowMonitor(window, NULL, (max_width / 2) - (773 / 2), (max_hieght / 2) - (684 / 2), 773, 684, GLFW_DONT_CARE);
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
	

	UI_ELEM_LIST listr;
	
	listr.add_elem(std::make_unique<UI_DIV>(500, 500, 100, 600, 4, glm::vec3(0, 1, 0)));
	listr.add_elem(std::make_unique<UI_IMG>(450, 450, 100, 600, "assets/loader_background.jpg", 15));
	listr.add_elem(std::make_unique<UI_FONT>(100, 200, glm::vec3(0.34509803921568627, 0.34901960784313724, 0.3568627450980392), 24, "assets/font/Roboto-Regular.ttf", "under", 6));
	
	listr.add_elem(std::make_unique<UI_FONT>(150, 200, glm::vec3(1, 0, 1), 24, "assets/font/Roboto-Regular.ttf", "upupupupupupupupupupapapapapapapaupupu", 2));
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		listr.render();
		glfwSwapBuffers(window);
		glfwPollEvents();

	}
	glfwTerminate();
	return;
}


void processInput(GLFWwindow* window)
{
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	static bool leftPressedLastFrame = false;
	int leftButtonState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	
	// hover local events

	leftPressedLastFrame = (leftButtonState == GLFW_PRESS);
}
