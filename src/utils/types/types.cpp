#include "types.h"
#include "../global/global.h"





UI_FONT::UI_FONT(int x_pos, int y_pos, glm::vec3 color, float scale, std::string font_name, std::string text, float z_index)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dist(0, 10000);
	this->id = dist(gen);
	this->x_pos = x_pos;
	this->y_pos = y_pos;
	this->color = color;
	this->scale = scale;
	this->text = text;
	this->z_index = z_index;
	this->font_name = font_name;
	this->font.init(font_name);
}
void UI_FONT::set_scale(float scale)
{
	this->scale = scale;
}
void UI_FONT::set_color(glm::vec3 color)
{
	this->color = color;
}
void UI_FONT::set_z_index(float z_index) {
	this->z_index = z_index;
}
void UI_FONT::draw()
{
	this->font.draw_text(this->text, static_cast<float>(x_pos), static_cast<float>(y_pos), this->scale, this->color, this->z_index);
}
void UI_FONT::set_x_pos(int x_pos)
{
	this->x_pos = x_pos;
}
void UI_FONT::set_y_pos(int y_pos)
{
	this->y_pos = y_pos;
}


void Font::draw_text(std::string text_display, float x_pos, float y_pos, float scale, glm::vec3 color, float z_index)
{
	if (text_display.size() == 0)
		text_display = std::to_string(this->id);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	Global::font_shader.use();
	Global::font_shader.setMat4("projection", pixel_simple_placement_projection);
	Global::font_shader.setVec3("color", color);
	Global::font_shader.setFloat("z_index", z_index);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(this->VAO);
	std::string::const_iterator c;
	for (c = text_display.begin(); c != text_display.end(); c++)
	{
		Character ch = Characters[*c];

		float xpos = x_pos + ch.Bearing.x * scale;
		float ypos = y_pos - (ch.Size.y - ch.Bearing.y) * scale;

		float w = ch.Size.x * scale;
		float h = ch.Size.y * scale;

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
		x_pos += (ch.Advance >> 6) * scale; 
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_BLEND);
	glDisable(GL_CULL_FACE);

}

Font::Font()
{

}

void Font::init(std::string font_name)
{
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
		FT_Set_Pixel_Sizes(face, 0, 48);
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


UI_IMG::UI_IMG(int width, int height, int x_pos, int y_pos, std::string texture_path, float z_index)
{
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
		static_cast<int>(x_pos), static_cast<int>(y_pos), z_index,								1.0f, 1.0f,
		static_cast<int>(x_pos + this->width), static_cast<int>(y_pos), z_index,				1.0f, 0.0f,
		static_cast<int>(x_pos + this->width), static_cast<int>(y_pos - this->height), z_index, 0.0f, 0.0f,
		static_cast<int>(x_pos), static_cast<int>(y_pos - this->height), z_index,				0.0f, 1.0f,
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
	Global::img_shader.use();
	Global::img_shader.setMat4("projection", this->projection);
	glBindTexture(GL_TEXTURE_2D, this->texture);
	glBindVertexArray(this->VAO);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}
int UI_IMG::get_id()
{
	return this->id;
}
int UI_IMG::get_width()
{
	return this->width;
}
int UI_IMG::get_height()
{
	return this->height;
}
int UI_IMG::get_x_pos()
{
	return this->x_pos;
}
int UI_IMG::get_y_pos()
{
	return this->y_pos;
}
float UI_IMG::get_z_index()
{
	return this->z_index;
}
std::string UI_IMG::get_texture_img()
{
	return this->texture_path;
}
float UI_IMG::get_opacity()
{
	return this->opacity;
}
void UI_IMG::set_width(int width)
{
	this->width = width;
}
void UI_IMG::set_height(int height)
{
	this->height = height;
}
void UI_IMG::set_x_pos(int x_pos)
{
	this->x_pos = x_pos;
}
void UI_IMG::set_y_pos(int x_pos)
{
	this->y_pos = y_pos;
}
void UI_IMG::set_texture(std::string texture_path)
{
	this->texture = load_texture(texture_path.c_str());
}
void UI_IMG::set_z_index(float z_index)
{
	this->z_index = z_index;
}
void UI_IMG::set_opacity(float opacity)
{
	this->opacity = opacity;
}



void UI_BUTTON::set_height(int height)
{
	this->height = height;
}
void UI_BUTTON::set_width(int width)
{
	this->width = width;
}
void UI_BUTTON::set_on_click(void(*onclick)())
{
	this->onclick = onclick;
}
void UI_BUTTON::set_x_pos(int x_pos)
{
	this->x_pos = x_pos;
}
void UI_BUTTON::set_y_pos(int y_pos)
{
	this->y_pos = y_pos;
}
UI_BUTTON::UI_BUTTON(int width, int height, int x_pos, int y_pos, float z_index, void(*onclick)(), glm::vec3 background_color)
{
	this->width = width;
	this->height = height;
	this->x_pos = x_pos;
	this->y_pos = y_pos;
	this->z_index = z_index;
	this->onclick = onclick;
	this->background_color = background_color;
	float coords[12] = {
		static_cast<float>(this->x_pos), static_cast<float>(this->y_pos), z_index,
		static_cast<float>(this->x_pos + this->width), static_cast<float>(this->y_pos), z_index,
		static_cast<float>(this->x_pos + this->width), static_cast<float>(this->y_pos - this->height), z_index,
		static_cast<float>(this->x_pos), static_cast<float>(this->y_pos - this->width), z_index,
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

UI_ELEM_LIST::UI_ELEM_LIST()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dist(0, 10000);
	this->id = dist(gen);
}
void UI_ELEM_LIST::add_ui_img(UI_IMG elem)
{
	this->ui_img_list.push_back(elem);
}
void UI_ELEM_LIST::add_ui_button(UI_BUTTON elem)
{
	this->ui_button_list.push_back(elem);
}
void UI_ELEM_LIST::add_ui_font(UI_FONT elem)
{
	this->ui_font_list.push_back(elem);
}
void UI_ELEM_LIST::render()
{
	for (size_t i = 0; i < this->ui_img_list.size(); i++)
	{
		this->ui_img_list.at(i).draw();
	}
	for (size_t i = 0; i < this->ui_font_list.size(); i++)
	{
		this->ui_font_list.at(i).draw();
	}
	for (size_t i = 0; i < this->ui_button_list.size(); i++)
	{
		this->ui_button_list.at(i).draw();
	}
}