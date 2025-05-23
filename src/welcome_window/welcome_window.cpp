#include "welcome_window.h"


void char_callback(GLFWwindow* window, unsigned int codepoint);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
static int active_menu_item = 1;
GLFWwindow* window;
static UI_ELEM_LIST project_page_ui;
static UI_ELEM_LIST addons_page_ui;
static UI_ELEM_LIST environment_page_ui;
static UI_ELEM_LIST documentation_page_ui;

static UI_ELEM_LIST clone_repos_subpage_ui;
static UI_ELEM_LIST open_exist_project_subpage_ui;
static UI_ELEM_LIST create_new_project_subpage_ui;

static std::string location = "~opengui";
static std::string project_name = "test_01";

static bool clone_repos_subpage = false;
static bool open_exist_subpage = false;
static bool create_new_project_subpage = false;


void display_project_page()
{
	if (clone_repos_subpage)
	{
		clone_repos_subpage_ui.render();
	}
	else if (open_exist_subpage)
	{
		std::string new_project_name = select_file();
		if (new_project_name.size() >= 1)
		{
			working_path = new_project_name;
			project_name = new_project_name;
			open_exist_subpage = false;
		}
		else
		{
			open_exist_subpage = false;
		}
	}
	else if (create_new_project_subpage)
	{
		create_new_project_subpage_ui.render();
	}
	else
	{
		project_page_ui.render();
	}
}

void display_addons_page(bool flag)
{
	for (size_t i = 0; i < addons_page_ui.ui_elements.size(); i++)
	{
		addons_page_ui.ui_elements.at(i)->set_display(flag);
	}
}

void display_environment_page(bool flag)
{
	for (size_t i = 0; i < environment_page_ui.ui_elements.size(); i++)
	{
		environment_page_ui.ui_elements.at(i)->set_display(flag);
	}
}

void display_documentation_page(bool flag)
{
	for (size_t i = 0; i < documentation_page_ui.ui_elements.size(); i++)
	{
		documentation_page_ui.ui_elements.at(i)->set_display(flag);
	}
}




static UI_ELEM_LIST root_ui;
void processInput(GLFWwindow* window, int button, int action, int mods);

void actions_callback(GLFWwindow* window)
{
	double x, y;
	glfwGetCursorPos(window, &x, &y);
	// hover events
	if ((x == mouse_x_pos) && (y == mouse_y_pos))
		// mouse pos dont changed
	{
		
		// hover handler
	}
	else
	{
		mouse_x_pos = static_cast<int>(x);
		mouse_y_pos = static_cast<int>(y);
		for (size_t i = 0; i < root_ui.ui_elements.size(); i++)
		{
			root_ui.ui_elements.at(i)->hover_check(mouse_x_pos, mouse_y_pos);
		}
		
	}
}




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
	glfwSetKeyCallback(window, key_callback);
	glfwSetCharCallback(window, char_callback);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return;
	}
	Global::init_shaders();
	Global::compute_projections();
	root_ui.add_elem(std::make_unique<UI_FONT>(22, 659, glm::vec3(0.34509803921568627, 0.34901960784313724, 0.3568627450980392), 15, "assets/font/Roboto-Regular.ttf", "OpenGUI v0.1", 4));

	root_ui.add_elem(std::make_unique<UI_DIV>(WIDTH, 1, 22, 645, 11, glm::vec3(0.9137254901960784, 0.9137254901960784, 0.9137254901960784)));

	int minimize_button_passive = root_ui.add_elem(std::make_unique<UI_DIV>(37, 40, 682, HEIGHT, 20, glm::vec3(0.9647058823529412, 0.9647058823529412, 0.9647058823529412)));
	int close_button_passive = root_ui.add_elem(std::make_unique<UI_DIV>(39, 40, 724, HEIGHT, 20, glm::vec3(0.9647058823529412, 0.9647058823529412, 0.9647058823529412)));

	
	

	root_ui.edit_by_id<UI_DIV>(minimize_button_passive, [](UI_DIV& div) {
		div.set_onclick([]() {
			glfwIconifyWindow(window);
			});
		});
	root_ui.edit_by_id<UI_DIV>(close_button_passive, [](UI_DIV& div) {
		div.set_onclick([]() {
			exit(0);
			});
		});



	root_ui.add_elem(std::make_unique<UI_IMG>(14, 14, 737, 672, "assets/close.png", 22));
	root_ui.add_elem(std::make_unique<UI_IMG>(18, 1, 692, 665, "assets/minimize.png", 22));
	root_ui.add_elem(std::make_unique<UI_DIV>(WIDTH, 38, 0, HEIGHT, 1, glm::vec3(0.9647058823529412, 0.9647058823529412, 0.9647058823529412)));

	root_ui.add_elem(std::make_unique<UI_DIV>(WIDTH, 2, 0, 645, 3, glm::vec3(0.9137254901960784, 0.9137254901960784, 0.9137254901960784)));



	root_ui.add_elem(std::make_unique<UI_DIV>(220, 645, 0, 643, 4, glm::vec3(0.9647058823529412, 0.9647058823529412, 0.9647058823529412)));

	int projects_menu_button_id = root_ui.add_elem(std::make_unique<UI_DIV>(166, 38, 26, 610, 10, glm::vec3(0.9647058823529412, 0.9647058823529412, 0.9647058823529412)));
	int environment_menu_button_id = root_ui.add_elem(std::make_unique<UI_DIV>(166, 38, 26, 560, 10, glm::vec3(0.9647058823529412, 0.9647058823529412, 0.9647058823529412)));
	int addons_menu_button_id = root_ui.add_elem(std::make_unique<UI_DIV>(166, 38, 26, 510, 10, glm::vec3(0.9647058823529412, 0.9647058823529412, 0.9647058823529412)));
	int documentation_menu_button_id = root_ui.add_elem(std::make_unique<UI_DIV>(166, 38, 26, 460, 10, glm::vec3(0.9647058823529412, 0.9647058823529412, 0.9647058823529412)));

	int projects_menu_item_id = root_ui.add_elem(std::make_unique<UI_FONT>(40, 585, glm::vec3(0, 0, 0), 16, "assets/font/Roboto-Regular.ttf", "Projects", 11));
	int environment_item_id = root_ui.add_elem(std::make_unique<UI_FONT>(40, 535, glm::vec3(0, 0, 0), 16, "assets/font/Roboto-Regular.ttf", "Environment", 11));
	int addons_menu_item_id = root_ui.add_elem(std::make_unique<UI_FONT>(40, 485, glm::vec3(0, 0, 0), 16, "assets/font/Roboto-Regular.ttf", "Addons", 11));
	int documentation_menu_item_id = root_ui.add_elem(std::make_unique<UI_FONT>(40, 435, glm::vec3(0, 0, 0), 16, "assets/font/Roboto-Regular.ttf", "Documentation", 11));


	root_ui.edit_by_id<UI_DIV>(projects_menu_button_id, [](UI_DIV& div) {
		div.set_onclick([]() {
			active_menu_item = 1;
			clone_repos_subpage = false;
			open_exist_subpage = false;
			create_new_project_subpage = false;
			});
		});
	root_ui.edit_by_id<UI_DIV>(environment_menu_button_id, [](UI_DIV& div) {
		div.set_onclick([]() {
			active_menu_item = 2;
			clone_repos_subpage = false;
			open_exist_subpage = false;
			create_new_project_subpage = false;
			});
		});
	root_ui.edit_by_id<UI_DIV>(addons_menu_button_id, [](UI_DIV& div) {
		div.set_onclick([]() {
			active_menu_item = 3;
		    clone_repos_subpage = false;
			open_exist_subpage = false;
			create_new_project_subpage = false;
			});
		});
	root_ui.edit_by_id<UI_DIV>(documentation_menu_button_id, [](UI_DIV& div) {
		div.set_onclick([]() {
			active_menu_item = 4;
			clone_repos_subpage = false;
			open_exist_subpage = false;
			create_new_project_subpage = false;
			});
		});

	// documentation page 
	int documentation_page_title = documentation_page_ui.add_elem(std::make_unique<UI_FONT>(415, 344, glm::vec3(0, 0, 0), 24, "assets/font/Roboto-Medium.ttf", "Documentation", 20));
	int documentation_page_subtitle = documentation_page_ui.add_elem(std::make_unique<UI_FONT>(441, 316, glm::vec3(0, 0, 0), 16, "assets/font/Roboto-Regular.ttf", "In development", 20));
	//
	// addons page
	int addons_page_title = addons_page_ui.add_elem(std::make_unique<UI_FONT>(456, 344, glm::vec3(0, 0, 0), 24, "assets/font/Roboto-Medium.ttf", "Addons", 20));
	int addons_page_subtitle = addons_page_ui.add_elem(std::make_unique<UI_FONT>(441, 316, glm::vec3(0, 0, 0), 16, "assets/font/Roboto-Regular.ttf", "In development", 20));
	//
	// environment page
	int environment_page_title = environment_page_ui.add_elem(std::make_unique<UI_FONT>(429, 344, glm::vec3(0, 0, 0), 24, "assets/font/Roboto-Medium.ttf", "Environment", 20));
	int environmet_page_subtitle = environment_page_ui.add_elem(std::make_unique<UI_FONT>(441, 316, glm::vec3(0, 0, 0), 16, "assets/font/Roboto-Regular.ttf", "In development", 20));
	//



	// project page
	int project_page_title = project_page_ui.add_elem(std::make_unique<UI_FONT>(354, 511, glm::vec3(0, 0, 0), 34, "assets/font/Roboto-Medium.ttf", "Welcome OpenGUI", 20));
	int project_page_subtitle1 = project_page_ui.add_elem(std::make_unique<UI_FONT>(368, 443, glm::vec3(0.5607843137254902, 0.5607843137254902, 0.5607843137254902), 20, "assets/font/Roboto-Regular.ttf", "Create a new project to start", 20));
	int project_page_subtitle2 = project_page_ui.add_elem(std::make_unique<UI_FONT>(289, 399, glm::vec3(0.5607843137254902, 0.5607843137254902, 0.5607843137254902), 20, "assets/font/Roboto-Regular.ttf", "Open exist project from disk or version controll", 20));
	int create_new_project_button = project_page_ui.add_elem(std::make_unique<UI_DIV>(55, 60, 355, 329, 20, glm::vec3(0.9294117647058824, 0.9294117647058824, 0.9294117647058824)));
	int open_exist_project_button = project_page_ui.add_elem(std::make_unique<UI_DIV>(55, 60, 471, 329, 20, glm::vec3(0.9294117647058824, 0.9294117647058824, 0.9294117647058824)));
	int clone_project_button = project_page_ui.add_elem(std::make_unique<UI_DIV>(55, 60, 587, 329, 20, glm::vec3(0.9294117647058824, 0.9294117647058824, 0.9294117647058824)));

	int create_new_project_icon = project_page_ui.add_elem(std::make_unique<UI_IMG>(30, 30, 368, 314, "assets/new_project.png", 22));
	int open_exist_project_icon = project_page_ui.add_elem(std::make_unique<UI_IMG>(23, 23, 488, 309, "assets/open_exist_project.png", 22));
	int clone_project_icon = project_page_ui.add_elem(std::make_unique<UI_IMG>(27, 27, 601, 312, "assets/clone_repos.png", 22));


	int create_new_project_text = project_page_ui.add_elem(std::make_unique<UI_FONT>(345, 240, glm::vec3(0, 0, 0), 13, "assets/font/Roboto-Regular.ttf", "New project", 22));
	int open_project_text = project_page_ui.add_elem(std::make_unique<UI_FONT>(482, 240, glm::vec3(0, 0, 0), 13, "assets/font/Roboto-Regular.ttf", "Open", 22));
	int clone_project_text = project_page_ui.add_elem(std::make_unique<UI_FONT>(597, 240, glm::vec3(0, 0, 0), 13, "assets/font/Roboto-Regular.ttf", "Clone", 22));

	project_page_ui.edit_by_id<UI_DIV>(create_new_project_button, [](UI_DIV& div) {
		div.set_onclick([]() {
			create_new_project_subpage = true;
			});
		});
	project_page_ui.edit_by_id<UI_DIV>(open_exist_project_button, [](UI_DIV& div)	 {
		div.set_onclick([]() {
			open_exist_subpage = true;
			});
		});
	project_page_ui.edit_by_id<UI_DIV>(clone_project_button, [](UI_DIV& div) {
		div.set_onclick([]() {
			clone_repos_subpage = true;
			});
		});
	//

	// clone subpage
	int clone_subpage_main_icon = clone_repos_subpage_ui.add_elem(std::make_unique<UI_IMG>(35, 35, 248, 571, "assets/git_clone.png", 25));
	int clone_subpage_main_title = clone_repos_subpage_ui.add_elem(std::make_unique<UI_FONT>(293, 549, glm::vec3(0, 0, 0), 16, "assets/font/Roboto-Regular.ttf", "Repository URL", 25));
	int version_controll_label = clone_repos_subpage_ui.add_elem(std::make_unique<UI_FONT>(248, 495, glm::vec3(0, 0, 0), 16, "assets/font/Roboto-Regular.ttf", "Repository URL", 25));
	int clone_subpage_git_input = clone_repos_subpage_ui.add_elem(std::make_unique<UI_IMG>(86, 28, 382, 516, "assets/git_input.png", 25));

	
	
	//
	
	// new project subpage
	int new_project_main_icon = create_new_project_subpage_ui.add_elem(std::make_unique<UI_IMG>(37, 37, 248, 573, "assets/new_project_main_icon.png", 25));
	int new_project_main_title = create_new_project_subpage_ui.add_elem(std::make_unique<UI_FONT>(293, 549, glm::vec3(0, 0, 0), 16, "assets/font/Roboto-Regular.ttf", "New project", 25));
	
	int new_project_name_label = create_new_project_subpage_ui.add_elem(std::make_unique<UI_FONT>(248, 495, glm::vec3(0.26666666666666666, 0.26666666666666666, 0.26666666666666666), 16, "assets/font/Roboto-Regular.ttf", "Name: ", 25));
	int new_project_name_input = create_new_project_subpage_ui.add_elem(std::make_unique<UI_IMG>(390, 28, 355, 516, "assets/new_project_name_input.png", 25));

	int new_project_location_label = create_new_project_subpage_ui.add_elem(std::make_unique<UI_FONT>(248, 446, glm::vec3(0.26666666666666666, 0.26666666666666666, 0.26666666666666666), 16, "assets/font/Roboto-Regular.ttf", "Location: ", 25));
	int new_project_location_input = create_new_project_subpage_ui.add_elem(std::make_unique<UI_IMG>(390, 28, 355, 463, "assets/new_project_name_input.png", 25));

	int new_project_create_button = create_new_project_subpage_ui.add_elem(std::make_unique<UI_DIV>(86, 35, 546, 60, 25, glm::vec3(0.09411764705882353, 0.3764705882352941, 0.796078431372549)));
	int new_project_create_text = create_new_project_subpage_ui.add_elem(std::make_unique<UI_FONT>(565, 37, glm::vec3(1, 1, 1), 16, "assets/font/Roboto-Regular.ttf", "Create", 26));

	int new_project_cancle_button = create_new_project_subpage_ui.add_elem(std::make_unique<UI_DIV>(86, 35, 647, 60, 25, glm::vec3(0.9372549019607843, 0.9254901960784314, 0.9254901960784314)));
	int new_project_cancel_text = create_new_project_subpage_ui.add_elem(std::make_unique<UI_FONT>(667, 37, glm::vec3(0, 0, 0), 16, "assets/font/Roboto-Regular.ttf", "Cancel", 26));

	create_new_project_subpage_ui.edit_by_id<UI_DIV>(new_project_create_button, [](UI_DIV& div) {
		div.set_onclick([]() {
			std::cout << location.size() << std::endl;
			if (location.size() >= 1)
			{
				working_path = location;
				project_name = project_name;
			}
			});
		});
	create_new_project_subpage_ui.edit_by_id<UI_DIV>(new_project_cancle_button, [](UI_DIV& div) {
		div.set_onclick([]() {
			create_new_project_subpage = false;
			});
		});

	int new_project_select_folder = create_new_project_subpage_ui.add_elem(std::make_unique<UI_IMG>(18, 18, 715, 458, "assets/folder.png", 25));

	create_new_project_subpage_ui.edit_by_id<UI_IMG>(new_project_select_folder, [](UI_IMG& img) {
		img.set_onclick([]() {
			std::string new_location = select_folder();
			if (new_location.size() == 0) {
				return;
			}
			else
			{
				location = new_location;
			}
			});
		});
	int new_project_name_input_value = create_new_project_subpage_ui.add_elem(std::make_unique<UI_FONT>(381, 498, glm::vec3(0, 0, 0), 15, "assets/font/Roboto-Regular.ttf", project_name, 27));
	int new_project_location_input_value = create_new_project_subpage_ui.add_elem(std::make_unique<UI_FONT>(381, 445, glm::vec3(0, 0, 0), 15, "assets/font/Roboto-Regular.ttf", location, 27));
	//



	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	while (!glfwWindowShouldClose(window) && working_path.size() == 0)
	{
		
		actions_callback(window);
		glfwSetMouseButtonCallback(window, processInput);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		root_ui.render();
		switch (active_menu_item)
		{
		case 1: {
			display_project_page();
			create_new_project_subpage_ui.edit_by_id<UI_FONT>(new_project_name_input_value, [](UI_FONT& font) {
				if (project_name.size() == 0)
				{
					project_name = " ";
				}
				font.text = project_name;
				});
			create_new_project_subpage_ui.edit_by_id<UI_FONT>(new_project_location_input_value, [](UI_FONT& font) {
				if (location.size() == 0)
				{
					location = " ";
				}
				font.text = location;
				});
			root_ui.edit_by_id<UI_DIV>(projects_menu_button_id, [](UI_DIV& div) {
				div.background_color = glm::vec3(0, 0.49411764705882355, 1);
				});
			root_ui.edit_by_id<UI_DIV>(environment_menu_button_id, [](UI_DIV& div) {
				div.background_color = glm::vec3(0.9647058823529412, 0.9647058823529412, 0.9647058823529412);
				});
			root_ui.edit_by_id<UI_DIV>(addons_menu_button_id, [](UI_DIV& div) {
				div.background_color = glm::vec3(0.9647058823529412, 0.9647058823529412, 0.9647058823529412);
				});
			root_ui.edit_by_id<UI_DIV>(documentation_menu_button_id, [](UI_DIV& div) {
				div.background_color = glm::vec3(0.9647058823529412, 0.9647058823529412, 0.9647058823529412);
				});
			root_ui.edit_by_id<UI_FONT>(projects_menu_item_id, [](UI_FONT& font) {
				font.color = glm::vec3(1, 1, 1);
				});
			root_ui.edit_by_id<UI_FONT>(environment_item_id, [](UI_FONT& font) {
				font.color = glm::vec3(0, 0, 0);
				});
			root_ui.edit_by_id<UI_FONT>(addons_menu_item_id, [](UI_FONT& font) {
				font.color = glm::vec3(0, 0, 0);
				});
			root_ui.edit_by_id<UI_FONT>(documentation_menu_item_id, [](UI_FONT& font) {
				font.color = glm::vec3(0, 0, 0);
				});
			break;
		};
		case 2: {
			environment_page_ui.render();
			root_ui.edit_by_id<UI_DIV>(projects_menu_button_id, [](UI_DIV& div) {
				div.background_color = glm::vec3(0.9647058823529412, 0.9647058823529412, 0.9647058823529412);
				});
			root_ui.edit_by_id<UI_DIV>(environment_menu_button_id, [](UI_DIV& div) {
				div.background_color = glm::vec3(0, 0.49411764705882355, 1);
				});
			root_ui.edit_by_id<UI_DIV>(addons_menu_button_id, [](UI_DIV& div) {
				div.background_color = glm::vec3(0.9647058823529412, 0.9647058823529412, 0.9647058823529412);
				});
			root_ui.edit_by_id<UI_DIV>(documentation_menu_button_id, [](UI_DIV& div) {
				div.background_color = glm::vec3(0.9647058823529412, 0.9647058823529412, 0.9647058823529412);
				});
			root_ui.edit_by_id<UI_FONT>(projects_menu_item_id, [](UI_FONT& font) {
				font.color = glm::vec3(0, 0, 0);
				});
			root_ui.edit_by_id<UI_FONT>(environment_item_id, [](UI_FONT& font) {
				font.color = glm::vec3(1, 1, 1);
				});
			root_ui.edit_by_id<UI_FONT>(addons_menu_item_id, [](UI_FONT& font) {
				font.color = glm::vec3(0, 0, 0);
				});
			root_ui.edit_by_id<UI_FONT>(documentation_menu_item_id, [](UI_FONT& font) {
				font.color = glm::vec3(0, 0, 0);
				});
			break;
		}
		case 3: {
			addons_page_ui.render();
			root_ui.edit_by_id<UI_DIV>(projects_menu_button_id, [](UI_DIV& div) {
				div.background_color = glm::vec3(0.9647058823529412, 0.9647058823529412, 0.9647058823529412);
				});
			root_ui.edit_by_id<UI_DIV>(environment_menu_button_id, [](UI_DIV& div) {
				div.background_color = glm::vec3(0.9647058823529412, 0.9647058823529412, 0.9647058823529412);
				});
			root_ui.edit_by_id<UI_DIV>(addons_menu_button_id, [](UI_DIV& div) {
				div.background_color = glm::vec3(0, 0.49411764705882355, 1);
				});
			root_ui.edit_by_id<UI_DIV>(documentation_menu_button_id, [](UI_DIV& div) {
				div.background_color = glm::vec3(0.9647058823529412, 0.9647058823529412, 0.9647058823529412);
				});
			root_ui.edit_by_id<UI_FONT>(projects_menu_item_id, [](UI_FONT& font) {
				font.color = glm::vec3(0, 0, 0);
				});
			root_ui.edit_by_id<UI_FONT>(environment_item_id, [](UI_FONT& font) {
				font.color = glm::vec3(0, 0, 0);
				});
			root_ui.edit_by_id<UI_FONT>(addons_menu_item_id, [](UI_FONT& font) {
				font.color = glm::vec3(1, 1, 1);
				});
			root_ui.edit_by_id<UI_FONT>(documentation_menu_item_id, [](UI_FONT& font) {
				font.color = glm::vec3(0, 0, 0);
				});
			break;
		}
		case 4: {
			documentation_page_ui.render();
			root_ui.edit_by_id<UI_DIV>(projects_menu_button_id, [](UI_DIV& div) {
				div.background_color = glm::vec3(0.9647058823529412, 0.9647058823529412, 0.9647058823529412);
				});
			root_ui.edit_by_id<UI_DIV>(environment_menu_button_id, [](UI_DIV& div) {
				div.background_color = glm::vec3(0.9647058823529412, 0.9647058823529412, 0.9647058823529412);
				});
			root_ui.edit_by_id<UI_DIV>(addons_menu_button_id, [](UI_DIV& div) {
				div.background_color = glm::vec3(0.9647058823529412, 0.9647058823529412, 0.9647058823529412);
				});
			root_ui.edit_by_id<UI_DIV>(documentation_menu_button_id, [](UI_DIV& div) {
				div.background_color = glm::vec3(0, 0.49411764705882355, 1);
				});
			root_ui.edit_by_id<UI_FONT>(projects_menu_item_id, [](UI_FONT& font) {
				font.color = glm::vec3(0, 0, 0);
				});
			root_ui.edit_by_id<UI_FONT>(environment_item_id, [](UI_FONT& font) {
				font.color = glm::vec3(0, 0, 0);
				});
			root_ui.edit_by_id<UI_FONT>(addons_menu_item_id, [](UI_FONT& font) {	
				font.color = glm::vec3(0, 0, 0);
				});
			root_ui.edit_by_id<UI_FONT>(documentation_menu_item_id, [](UI_FONT& font) {
				font.color = glm::vec3(1, 1, 1);
				});
			break;
		}
		default:
			break;
		}
		
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
			for (size_t i = 0; i < root_ui.ui_elements.size(); i++)
			{
				root_ui.ui_elements.at(i)->click_check(mouse_x_pos, mouse_y_pos);
			}
			for (size_t i = 0; i < project_page_ui.ui_elements.size(); i++)
			{
				project_page_ui.ui_elements.at(i)->click_check(mouse_x_pos, mouse_y_pos);
			}
			for (size_t i = 0; i < addons_page_ui.ui_elements.size(); i++)
			{
				addons_page_ui.ui_elements.at(i)->click_check(mouse_x_pos, mouse_y_pos);
			}
			for (size_t i = 0; i < documentation_page_ui.ui_elements.size(); i++)
			{
				documentation_page_ui.ui_elements.at(i)->click_check(mouse_x_pos, mouse_y_pos);
			}
			for (size_t i = 0; i < environment_page_ui.ui_elements.size(); i++)
			{
				environment_page_ui.ui_elements.at(i)->click_check(mouse_x_pos, mouse_y_pos);
			}
			for (size_t i = 0; i < clone_repos_subpage_ui.ui_elements.size(); i++)
			{
				clone_repos_subpage_ui.ui_elements.at(i)->click_check(mouse_x_pos, mouse_y_pos);
			}
			for (size_t i = 0; i < create_new_project_subpage_ui.ui_elements.size(); i++)
			{
				create_new_project_subpage_ui.ui_elements.at(i)->click_check(mouse_x_pos, mouse_y_pos);
			}
		}
		else if (action == GLFW_RELEASE) {
			// Обработка отпускания
			
		}
	}
}


void char_callback(GLFWwindow* window, unsigned int codepoint) {
	if (codepoint != ' ' && codepoint != '\t' && project_name.size() < 20) {
		project_name += static_cast<char>(codepoint);
	}
	
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS || action == GLFW_REPEAT) {
		if (key == GLFW_KEY_BACKSPACE && !project_name.empty()) {
			project_name.pop_back(); // Удаляем последний символ
		}
	}
	if (project_name.size() == 0)
		project_name = " ";
}