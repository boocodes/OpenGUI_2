#pragma once

#include <string>
#include <vector>


#include <glad/glad.h>
#include <glm/glm.hpp>

#include <fstream>
#include <sstream>
#include <iostream>

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
    std::string font_name;
    unsigned int VAO, VBO, texture;
    std::map<GLchar, Character> Characters;
public:
    Font();
    void draw_text(std::string text_display = "", float x_pos = 0, float y_pos = 20, float scale = 0.4f, glm::vec3 color = glm::vec3(0, 0, 0), float z_index = 0.0f);
    void init(std::string font_name);
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




class UI_IMG {
private:
    int id;
    float opacity;
    int width, height, x_pos, y_pos, z_index;
    std::string texture_path;
    unsigned int VAO, VBO, texture;
    glm::mat4 projection;
public:
    void set_width(int width);
    void set_height(int height);
    void set_x_pos(int x_pos);
    void set_y_pos(int y_pos);
    void set_z_index(float z_index);
    void set_texture(std::string texture_path);
    void set_opacity(float opacity);


    int get_id();
    int get_width();
    int get_height();
    int get_x_pos();
    int get_y_pos();
    float get_z_index();
    std::string get_texture_img();
    float get_opacity();
    UI_IMG(int width, int height, int x_pos, int y_pos, std::string texture_path, float z_index);
    void draw();
};

class UI_FONT {
private:
    int id;
    glm::vec3 color;
    int x_pos, y_pos;
    float z_index = 0.0f;
    Font font;
    float scale = 0.4f;
    std::string text;
    std::string font_name;
public:
    void draw();
    void set_scale(float scale);
    void set_x_pos(int x_pos);
    void set_y_pos(int y_pos);
    void set_color(glm::vec3 color);
    void set_z_index(float z_index);
    UI_FONT(int x_pos = 0, int y_pos = 30, glm::vec3 color = glm::vec3(0, 0, 0), float scale = 0.4, std::string font_name = "", std::string text = "", float z_index = 0.0f);
};