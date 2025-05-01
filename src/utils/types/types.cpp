#include "types.h"
#include "../global/global.h"
#include "algorithm"


// UI FONT ELEM

UI_FONT::UI_FONT(int x_pos, int y_pos, glm::vec3 color, int pixel_size, std::string font_name, std::string text, int z_index)
{
	this->pixel_size_px = pixel_size;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dist(0, 10000);
	this->id = dist(gen);
	this->x_pos = x_pos;
	this->y_pos = y_pos;
	this->color = color;
	this->text = text;
	this->z_index = z_index;
	this->font_name = font_name;
	this->font.init(font_name, pixel_size_px);
}

void UI_FONT::draw()
{
	this->font.draw_text(this->text, static_cast<float>(x_pos), static_cast<float>(y_pos), this->color);
}


// FONT GENERAL CLASS

void Font::draw_text(std::string text_display, float x_pos, float y_pos, glm::vec3 color)
{
	if (text_display.size() == 0)
		text_display = std::to_string(this->id);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
	Global::font_shader.use();
	Global::font_shader.setMat4("projection", pixel_simple_placement_projection);
	Global::font_shader.setVec3("textColor", color);
	Global::font_shader.setFloat("z_index", 0.0);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(this->VAO);
	std::string::const_iterator c;
	for (c = text_display.begin(); c != text_display.end(); c++)
	{
		Character ch = Characters[*c];

		float xpos = x_pos + ch.Bearing.x;
		float ypos = y_pos - (ch.Size.y - ch.Bearing.y);

		float w = ch.Size.x;
		float h = ch.Size.y;

		float vertices[6][4] = {
			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos,     ypos,       0.0f, 1.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },

			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },
			{ xpos + w, ypos + h,   1.0f, 0.0f }
		};
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		x_pos += (ch.Advance >> 6); 
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_BLEND);
	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
}
Font::Font()
{

}

void Font::init(std::string font_name, int pixel_size_px)
{
	this->pixel_size_px = pixel_size_px;
	this->font_name = font_name;
	Global::font_shader.use();
	Global::font_shader.setMat4("projection", pixel_simple_placement_projection);
	FT_Library ft;
	if (FT_Init_FreeType(&ft))
	{
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
		return;
	}

	if (this->font_name.empty())
	{
		std::cout << "ERROR::FREETYPE: Failed to load font_name" << std::endl;
		return;
	}
	
	FT_Face face;
	if (FT_New_Face(ft, font_name.c_str(), 0, &face)) {
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
		return;
	}
	else {
		FT_Set_Pixel_Sizes(face, 0, this->pixel_size_px);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		for (unsigned int c = 0; c < 256; c++)
		{
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			{
				std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
				continue;
			}

			glGenTextures(1, &this->texture);
			glBindTexture(GL_TEXTURE_2D, this->texture);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RED,
				face->glyph->bitmap.width,
				face->glyph->bitmap.rows,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				face->glyph->bitmap.buffer
			);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			Character character = {
				texture,
				glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				static_cast<unsigned int>(face->glyph->advance.x)
			};
			Characters.insert(std::pair<char, Character>(c, character));
		}
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	
}

// UI IMG ELEM

UI_IMG::UI_IMG(int width, int height, int x_pos, int y_pos, std::string texture_path, int z_index)
{
	this->opacity = 1.0f;	
	this->type = "img";
	this->texture_path = texture_path;
	this->width = width;
	this->height = height;
	this->x_pos = x_pos;
	this->y_pos = y_pos;
	this->z_index = z_index;
	this->projection = pixel_simple_placement_projection;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dist(0, 10000);
	this->id = dist(gen);
	this->texture = load_texture(this->texture_path.c_str());

	float coords[20] = {
		static_cast<int>(x_pos), static_cast<int>(y_pos), 0.0,								1.0f, 1.0f,
		static_cast<int>(x_pos + this->width), static_cast<int>(y_pos), 0.0,				    1.0f, 0.0f,
		static_cast<int>(x_pos + this->width), static_cast<int>(y_pos - this->height), 0.0,   0.0f, 0.0f,
		static_cast<int>(x_pos), static_cast<int>(y_pos - this->height), 0.0,				    0.0f, 1.0f,
	};

	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(coords), coords, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);
}
void UI_IMG::draw()
{
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	Global::img_shader.use();
	Global::img_shader.setMat4("projection", this->projection);
	glBindTexture(GL_TEXTURE_2D, this->texture);
	glBindVertexArray(this->VAO);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glDisable(GL_BLEND);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glEnable(GL_DEPTH_TEST);
}




void UI_IMG::set_texture(std::string texture_path)
{
	this->texture = load_texture(texture_path.c_str());
}

// UI BUTTON ELEM


UI_BUTTON::UI_BUTTON(int width, int height, int x_pos, int y_pos, int z_index, void(*onclick)(), glm::vec3 background_color)
{
	this->type = "button";
	this->width = width;
	this->height = height;
	this->x_pos = x_pos;
	this->y_pos = y_pos;
	this->z_index = z_index;
	this->onclick = onclick;
	this->background_color = background_color;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dist(0, 10000);
	this->id = dist(gen);
	float coords[12] = {
		static_cast<float>(this->x_pos), static_cast<float>(this->y_pos), 0.0,
		static_cast<float>(this->x_pos + this->width), static_cast<float>(this->y_pos), 0.0,
		static_cast<float>(this->x_pos + this->width), static_cast<float>(this->y_pos - this->height), 0.0,
		static_cast<float>(this->x_pos), static_cast<float>(this->y_pos - this->width), 0.0,
	};
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(coords), coords, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
	glEnableVertexAttribArray(0);
}
void UI_BUTTON::draw()
{
	Global::pixel_placement_shader.use();
	Global::pixel_placement_shader.setVec3("color", this->background_color);
	Global::pixel_placement_shader.setMat4("projection", pixel_simple_placement_projection);

	glBindVertexArray(this->VAO);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}
void UI_BUTTON::resize(int width, int height)
{
	this->width = width;
	this->height = height;
}
void UI_BUTTON::move(int x_pos, int y_pos)
{
	this->x_pos = x_pos;
	this->y_pos = y_pos;
}
void UI_BUTTON::set_background_color(glm::vec3 background_color)
{
	this->background_color = background_color;
}


//UI ELEM LIST

UI_ELEM_LIST::UI_ELEM_LIST()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dist(0, 10000);
	this->id = dist(gen);
}
void UI_ELEM_LIST::render()
{
	if (is_changed) {
		// Сортируем элементы по возрастанию z_index
		std::sort(
			ui_elements.begin(),
			ui_elements.end(),
			[](const std::unique_ptr<UI_ELEMENT>& a, const std::unique_ptr<UI_ELEMENT>& b) {
				return a->get_z_index() < b->get_z_index(); // Сортировка по возрастанию
			}
		);
		is_changed = false;
		// Вывод z_index для проверки
		for (const auto& elem : ui_elements) {
			std::cout << "z_index: " << elem->get_z_index() << std::endl;
		}
	}

	// Отрисовка в отсортированном порядке
	for (const auto& elem : ui_elements) {
		elem->draw();
	}
}
void UI_ELEM_LIST::add_elem(std::unique_ptr<UI_ELEMENT> elem)
{
	this->ui_elements.push_back(std::move(elem));
	this->is_changed = true;
}



// UI DIV ELEM

UI_DIV::UI_DIV(int width, int height, int x_pos, int y_pos, int z_index, glm::vec3 background_color)
{
	this->onhover = []() {};
	std::string type = "div";
	std::random_device rd;
	this->onclick = []() {};
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dist(0, 10000);
	this->id = dist(gen);
	this->width = width;
	this->height = height;
	this->x_pos = x_pos;
	this->y_pos = y_pos;
	this->z_index = z_index;
	this->background_color = background_color;
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	float coords[12] = {
		static_cast<float>(this->x_pos), static_cast<float>(this->y_pos), 0.0,
		static_cast<float>(this->x_pos + this->width), static_cast<float>(this->y_pos), 0.0,
		static_cast<float>(this->x_pos + this->width), static_cast<float>(this->y_pos - this->height), 0.0,
		static_cast<float>(this->x_pos), static_cast<float>(y_pos - this->height), 0.0,
	};
	for (size_t i = 0; i < 12; i++)
	{
		this->coords[i] = coords[i];
	}
	glBufferData(GL_ARRAY_BUFFER, sizeof(coords), coords, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
	glEnableVertexAttribArray(0);
}

bool UI_DIV::check_mouse_coords_equal(int mouse_x, int mouse_y)
{
	if (
		(mouse_x >= this->coords[0] && mouse_x <= this->coords[3] && mouse_x <= this->coords[6] && mouse_x >= this->coords[9])
		&&
		(HEIGHT - mouse_y <= this->coords[1] && HEIGHT - mouse_y <= this->coords[4] && HEIGHT - mouse_y >= this->coords[7] && HEIGHT - mouse_y >= this->coords[11])
		)
	{
		return true;
	}
	return false;
}
void UI_DIV::resize(int width, int height)
{
	this->width = width;
	this->height = height;
	float coords[12] = {
		static_cast<float>(this->x_pos), static_cast<float>(this->y_pos), 0.0,
		static_cast<float>(this->x_pos + this->width), static_cast<float>(this->y_pos), 0.0,
		static_cast<float>(this->x_pos + this->width), static_cast<float>(this->y_pos - this->height), 0.0,
		static_cast<float>(this->x_pos), static_cast<float>(this->y_pos - this->height), 0.0,
	};
	for (size_t i = 0; i < 12; i++)
	{
		this->coords[i] = coords[i];
	}
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(coords), coords);
}
void UI_DIV::move(int x_pos, int y_pos)
{
	this->x_pos = x_pos;
	this->y_pos = y_pos;
	float coords[12] = {
		static_cast<float>(this->x_pos), static_cast<float>(this->y_pos), 0.0,
		static_cast<float>(this->x_pos + this->width), static_cast<float>(this->y_pos), 0.0,
		static_cast<float>(this->x_pos + this->width), static_cast<float>(this->y_pos - this->height), 0.0,
		static_cast<float>(this->x_pos), static_cast<float>(this->y_pos - this->height), 0.0,
	};
	for (size_t i = 0; i < 12; i++)
	{
		this->coords[i] = coords[i];
	}
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(coords), coords);
}
void UI_DIV::draw()
{
	Global::pixel_placement_shader.use();
	Global::pixel_placement_shader.setMat4("projection", pixel_simple_placement_projection);
	Global::pixel_placement_shader.setVec3("color", this->background_color);
	glBindVertexArray(this->VAO);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glBindVertexArray(0);
}
void UI_DIV::centred()
{
	float new_x_pos = (WIDTH - this->width) / 2;
	this->x_pos = (WIDTH - this->width) / 2;
	float coords[12] = {
		new_x_pos, static_cast<float>(y_pos), 0.0,
		new_x_pos + this->width, static_cast<float>(this->y_pos), 0.0,
		new_x_pos + this->width, static_cast<float>(this->y_pos - this->height), 0.0,
		new_x_pos, static_cast<float>(this->y_pos - this->height), 0.0,
	};
	for (size_t i = 0; i < 12; i++)
	{
		this->coords[i] = coords[i];
	}
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(coords), coords);
}
void UI_DIV::padding_bottom(int padding)
{
	float new_y_pos = static_cast<float>(this->y_pos + padding);
	this->y_pos = this->y_pos + padding;
	float coords[12] = {
		static_cast<float>(this->x_pos), new_y_pos, 0.0,
		static_cast<float>(this->x_pos + this->width), new_y_pos, 0.0,
		static_cast<float>(this->x_pos + this->width), static_cast<float>(new_y_pos - this->height), 0.0,
		static_cast<float>(this->x_pos), static_cast<float>(new_y_pos - this->height), 0.0,
	};
	for (size_t i = 0; i < 12; i++)
	{
		this->coords[i] = coords[i];
	}
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(coords), coords);
}
void UI_DIV::padding_top(int padding)
{
	float new_y_pos = static_cast<float>(this->y_pos - padding);
	this->y_pos = this->y_pos - padding;
	float coords[12] = {
		static_cast<float>(this->x_pos), new_y_pos, 0.0,
		static_cast<float>(this->x_pos + this->width), new_y_pos, 0.0,
		static_cast<float>(this->x_pos + this->width), static_cast<float>(new_y_pos - this->height), 0.0,
		static_cast<float>(this->x_pos), static_cast<float>(new_y_pos - this->height), 0.0,
	};
	for (size_t i = 0; i < 12; i++)
	{
		this->coords[i] = coords[i];
	}
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(coords), coords);
}
void UI_DIV::padding_left(int padding)
{
	float new_x_pos = static_cast<float>(this->x_pos + padding);
	this->x_pos = this->y_pos + padding;
	float coords[12] = {
		new_x_pos, static_cast<float>(this->y_pos), 0.0,
		static_cast<float>(new_x_pos + this->width), static_cast<float>(this->y_pos), 0.0,
		static_cast<float>(new_x_pos + this->width), static_cast<float>(this->y_pos - this->height), 0.0,
		new_x_pos, static_cast<float>(this->y_pos - this->height), 0.0,
	};
	for (size_t i = 0; i < 12; i++)
	{
		this->coords[i] = coords[i];
	}
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(coords), coords);
}
void UI_DIV::padding_right(int padding)
{
	float new_x_pos = static_cast<float>(this->x_pos - padding);
	this->x_pos = this->x_pos - padding;
	float coords[12] = {
		new_x_pos, static_cast<float>(this->y_pos), 0.0,
		static_cast<float>(new_x_pos + this->width), static_cast<float>(this->y_pos), 0.0,
		static_cast<float>(new_x_pos + this->width), static_cast<float>(this->y_pos - this->height), 0.0,
		new_x_pos, static_cast<float>(this->y_pos - this->height), 0.0,
	};
	for (size_t i = 0; i < 12; i++)
	{
		this->coords[i] = coords[i];
	}
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(coords), coords);
}