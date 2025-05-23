#pragma once

#include <string>
#include <vector>
#include <functional>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <fstream>
#include <sstream>
#include <iostream>
#include <GLFW/glfw3.h>
#include <map>
#include <random>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ft2build.h>
#include "../methods/methods.h"
#include FT_FREETYPE_H


struct Character {
    unsigned int TextureID;
    glm::ivec2   Size;
    glm::ivec2   Bearing;
    unsigned int Advance;
};



class Font {
private:
    int id;
    int pixel_size_px = 48;
    std::string font_name;
    unsigned int VAO, VBO, texture;
    std::map<GLchar, Character> Characters;
public:
    Font();
    void draw_text(std::string text_display = "", float x_pos = 0, float y_pos = 20, glm::vec3 color = glm::vec3(0, 0, 0));
    void init(std::string font_name, int pixel_size_px = 48);
};


class Shader
{
public:
    unsigned int ID;
    Shader()
    {
        return;
    }
    void generate(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr)
    {
        std::string vertexCode;
        std::string fragmentCode;
        std::string geometryCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        std::ifstream gShaderFile;

        // Óáåæäàåìñÿ, ÷òî îáúåêòû ifstream ìîãóò âûáðîñèòü èñêëþ÷åíèå:
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            // Îòêðûâàåì ôàéëû
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;

            // ×èòàåì ñîäåðæèìîå ôàéëîâûõ áóôåðîâ
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();

            // Çàêðûâàåì ôàéëû
            vShaderFile.close();
            fShaderFile.close();

            // Êîíâåðòèðóåì â ñòðîêîâóþ ïåðåìåííóþ äàííûå èç ïîòîêà
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();

            // Åñëè äàí ïóòü ê ãåîìåòðè÷åñêîìó øåéäåðó, òî çàãðóæàåì è åãî
            if (geometryPath != nullptr)
            {
                gShaderFile.open(geometryPath);
                std::stringstream gShaderStream;
                gShaderStream << gShaderFile.rdbuf();
                gShaderFile.close();
                geometryCode = gShaderStream.str();
            }
        }
        catch (std::ifstream::failure& e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();

        // 2. Êîìïèëèðóåì øåéäåðû
        unsigned int vertex, fragment;

        // Âåðøèííûé øåéäåð
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");

        // Ôðàãìåíòíûé øåéäåð
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");

        // Åñëè áûë äàí ãåîìåòðè÷åñêèé øåéäåð, òî êîìïèëèðóåì åãî
        unsigned int geometry;
        if (geometryPath != nullptr)
        {
            const char* gShaderCode = geometryCode.c_str();
            geometry = glCreateShader(GL_GEOMETRY_SHADER);
            glShaderSource(geometry, 1, &gShaderCode, NULL);
            glCompileShader(geometry);
            checkCompileErrors(geometry, "GEOMETRY");
        }

        // Øåéäåðíàÿ ïðîãðàììà
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        if (geometryPath != nullptr)
            glAttachShader(ID, geometry);
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");

        // Ïîñëå òîãî, êàê ìû ñâÿçàëè øåéäåðû ñ íàøåé ïðîãðàììîé, óäàëÿåì èõ, ò.ê. îíè íàì áîëüøå íå íóæíû
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        if (geometryPath != nullptr)
            glDeleteShader(geometry);
    }
    // Êîíñòðóêòîð ãåíåðèðóåò øåéäåð "íà ëåòó"
    Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr)
    {
        // 1. Ïîëó÷åíèå èñõîäíîãî êîäà âåðøèííîãî/ôðàãìåíòíîãî øåéäåðà
        std::string vertexCode;
        std::string fragmentCode;
        std::string geometryCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        std::ifstream gShaderFile;

        // Óáåæäàåìñÿ, ÷òî îáúåêòû ifstream ìîãóò âûáðîñèòü èñêëþ÷åíèå:
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            // Îòêðûâàåì ôàéëû
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;

            // ×èòàåì ñîäåðæèìîå ôàéëîâûõ áóôåðîâ
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();

            // Çàêðûâàåì ôàéëû
            vShaderFile.close();
            fShaderFile.close();

            // Êîíâåðòèðóåì â ñòðîêîâóþ ïåðåìåííóþ äàííûå èç ïîòîêà
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();

            // Åñëè äàí ïóòü ê ãåîìåòðè÷åñêîìó øåéäåðó, òî çàãðóæàåì è åãî
            if (geometryPath != nullptr)
            {
                gShaderFile.open(geometryPath);
                std::stringstream gShaderStream;
                gShaderStream << gShaderFile.rdbuf();
                gShaderFile.close();
                geometryCode = gShaderStream.str();
            }
        }
        catch (std::ifstream::failure& e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();

        // 2. Êîìïèëèðóåì øåéäåðû
        unsigned int vertex, fragment;

        // Âåðøèííûé øåéäåð
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");

        // Ôðàãìåíòíûé øåéäåð
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");

        // Åñëè áûë äàí ãåîìåòðè÷åñêèé øåéäåð, òî êîìïèëèðóåì åãî
        unsigned int geometry;
        if (geometryPath != nullptr)
        {
            const char* gShaderCode = geometryCode.c_str();
            geometry = glCreateShader(GL_GEOMETRY_SHADER);
            glShaderSource(geometry, 1, &gShaderCode, NULL);
            glCompileShader(geometry);
            checkCompileErrors(geometry, "GEOMETRY");
        }

        // Øåéäåðíàÿ ïðîãðàììà
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        if (geometryPath != nullptr)
            glAttachShader(ID, geometry);
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");

        // Ïîñëå òîãî, êàê ìû ñâÿçàëè øåéäåðû ñ íàøåé ïðîãðàììîé, óäàëÿåì èõ, ò.ê. îíè íàì áîëüøå íå íóæíû
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        if (geometryPath != nullptr)
            glDeleteShader(geometry);
    }

    // Àêòèâàöèÿ øåéäåðà
    void use() const
    {
        glUseProgram(ID);
    }

    // Ïîëåçíûå uniform-ôóíêöèè
    // ------------------------------------------------------------------------
    void setBool(const std::string& name, bool value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }
    // ------------------------------------------------------------------------
    void setInt(const std::string& name, int value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void setFloat(const std::string& name, float value) const
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void setVec2(const std::string& name, const glm::vec2& value) const
    {
        glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
    void setVec2(const std::string& name, float x, float y) const
    {
        glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
    }
    // ------------------------------------------------------------------------
    void setVec3(const std::string& name, const glm::vec3& value) const
    {
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
    void setVec3(const std::string& name, float x, float y, float z) const
    {
        glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
    }
    // ------------------------------------------------------------------------
    void setVec4(const std::string& name, const glm::vec4& value) const
    {
        glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
    void setVec4(const std::string& name, float x, float y, float z, float w) const
    {
        glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
    }
    // ------------------------------------------------------------------------
    void setMat2(const std::string& name, const glm::mat2& mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat3(const std::string& name, const glm::mat3& mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat4(const std::string& name, const glm::mat4& mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

private:
    // Ïîëåçíûå ôóíêöèè äëÿ ïðîâåðêè îøèáîê êîìïèëÿöèè/ñâÿçûâàíèÿ øåéäåðîâ
    void checkCompileErrors(GLuint shader, std::string type)
    {
        GLint success;
        GLchar infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }
};



class UI_ELEMENT {
public:
    virtual ~UI_ELEMENT() = default;
    virtual void draw() = 0;
    virtual int get_z_index() = 0;
    virtual void set_display(bool flag) = 0;
    std::string class_name;
    std::string type;
    virtual bool click_check(int mouse_x, int mouse_y) = 0;
    std::string get_class_name() { return this->class_name; };
    virtual int get_id() = 0;
    virtual bool hover_check(int mouse_x_pos, int mouse_y_pos) = 0;
};


class UI_IMG : public UI_ELEMENT{
public:
    int id;
    std::string type = "img";
    float opacity;
    float coords[12];
    glm::vec3 color;
    bool display = true;
    std::function<void()> onclick = 0;
    void set_onclick(std::function<void()> onclick_new) { this->onclick = onclick_new; };
    void set_display(bool flag) { this->display = flag; };
    bool click_check(int mouse_x, int mouse_y) override;
    bool hover_check(int mouse_x_pos, int mouse_y_pos) override { return 1; };
    int width, height, x_pos, y_pos, z_index;
    std::string texture_path;
    int get_id() override { return this->id; };
    unsigned int VAO, VBO, texture;
    glm::mat4 projection;
    void set_texture(std::string texture_path);
    UI_IMG(int width, int height, int x_pos, int y_pos, std::string texture_path, int z_index);
    void draw() override;
    int get_z_index() override{ return this->z_index; };

};

class UI_FONT : public UI_ELEMENT{
public:
    int id;
    std::string type = "font";
    glm::vec3 color;
    int x_pos, y_pos;
    bool display = true;
    void set_display(bool flag) { this->display = flag; };
    bool click_check(int mouse_x, int mouse_y) { return false; };
    int z_index;
    bool hover_check(int mouse_x_pos, int mouse_y_pos) override { return 1; };
    Font font;
    int pixel_size_px = 24;
    void set_pixel_size(int pixel_size);
    int get_id() override { return this->id; };
    std::string text;
    std::string font_name;
    int get_z_index() override { return this->z_index; };
    void draw() override;
    UI_FONT(int x_pos, int y_pos, glm::vec3 color, int pixel_size, std::string font_name, std::string text, int z_index);
};

class UI_BUTTON : public UI_ELEMENT{
public:
    unsigned int VAO, VBO;
    bool hover_check(int mouse_x_pos, int mouse_y_pos) override { return 1; };
    int id;
    bool display = true;
    void set_display(bool flag) { this->display = flag; };
    std::string type = "button";
    bool click_check(int mouse_x, int mouse_y) { return false; };
    glm::vec3 background_color;
    void (*onclick)();
    int z_index;
    int get_id() override { return this->id; };
    int width, height,
        x_pos, y_pos;
    int get_z_index() override { return this->z_index; };
    UI_BUTTON(int width, int height, int x_pos, int y_pos, int z_index, void(*onclick)(), glm::vec3 background_color);
    void draw() override;
    void resize(int width, int height);
    void move(int x_pos, int y_pos);
    void set_background_color(glm::vec3 background_color);
};

class UI_DIV : public UI_ELEMENT{
public:
    std::string type = "div";
    bool display = true;
    void set_display(bool flag) { this->display = flag; };
    unsigned int VAO, VBO;
    int id;
    glm::vec4 radius = glm::vec4(5, 5, 5, 5);
    glm::vec3 background_color;
    int get_z_index() override { return this->z_index; };
    int z_index;
    std::function<void()> onhover;
    bool click_check(int mouse_x, int mouse_y) override;
    bool hover_check(int mouse_x_pos, int mouse_y_pos) override;
    bool hovered = false;
    std::function<void()> onclick;
    int get_id() override { return this->id; };
    std::function<void()> onmouse_over;
    float coords[12];
    int width, height,
        x_pos, y_pos;
    UI_DIV(int width, int height, int x_pos, int y_pos, int z_index, glm::vec3 background_color);
    void move(int x_pos, int y_pos);
    void resize(int width, int height);
    void draw() override;
    void centred();
    void padding_left(int padding); 
    void padding_right(int padding);
    void padding_bottom(int padding);
    void padding_top(int padding);
    void set_onclick(std::function<void()> onclick) { this->onclick = onclick; };
    void attract_onclick() { if (this->display) { this->onclick(); } };
    bool check_mouse_coords_equal(int mouse_x, int mouse_y);
    void set_onhover(std::function<void()> onhover) { this->onhover = onhover; };
    void attract_onhover() { this->onhover(); };
    void set_hovered(bool hovered) { this->hovered = hovered; };
};



class UI_ELEM_LIST {
public:
    int id;
    bool is_changed = false;
    std::vector<std::unique_ptr<UI_ELEMENT>> ui_elements;
    template <typename T, typename Func>
    void edit_by_id(int target_id, Func&& action) {
        for (auto&& elem : ui_elements) {
            if (elem->get_id() == target_id) {
                if (T* casted = dynamic_cast<T*>(elem.get())) {
                    action(*casted);
                    is_changed = true;
                    return;
                }
            }
        }
        throw std::runtime_error("Element not found or type mismatch");
    }
    UI_ELEMENT* get_element_by_id(int target_id) {
        for (const auto& elem : ui_elements) {
            if (elem->get_id() == target_id) {
                return elem.get();
            }
        }
        return nullptr;
    }
    UI_ELEM_LIST();
    void render();
    int add_elem(std::unique_ptr<UI_ELEMENT> elem);
};