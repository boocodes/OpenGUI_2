#include "loading_window.h"






void show_loading_window()
{
	/*UI_DOM ui_dom;
	ui_dom.add_ui_elem(std::make_unique<UI_IMG>(836, 600, 0, 0, "assets/loader_background.jpg", 0.0f));*/
	
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_DECORATED, false);



	GLFWwindow* window = glfwCreateWindow(836, 600, "OpenGUI", NULL, NULL);
	
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
	
	UI_FONT ui_font(100, 200, glm::vec3(0, 0, 0), 0.4f, "assets/font/Roboto-Regular.ttf", "OpenGUI", 0.0f);
	UI_IMG ui_img(836, 600, 0, 600, "assets/loader_background.jpg", 0.1f);
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

    while (!glfwWindowShouldClose(window))
    {
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		ui_img.draw();
		ui_font.draw();
        glfwSwapBuffers(window);
        glfwPollEvents();
		
    }


    glfwTerminate();
    return;

}


