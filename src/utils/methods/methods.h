#pragma once
#include <glad/glad.h>
#include <stb_image.h>
#include <iostream>
#include <string>
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../types/types.h"
#include <Windows.h>
#include <shobjidl.h> 

glm::vec3 hex_to_vec3(const std::string& hex);

unsigned int load_texture(const char* path);

std::wstring char_to_wstring(const char* str);
void open_url(const std::wstring& url);
std::string select_folder();
std::string select_file();
std::string wstrtostr(const std::wstring& wstr);

