#include "methods.h"
#include "../global/global.h"
#include "memory"
#include "vector"
#include "algorithm"
#include <tinyxml2.h>

using namespace tinyxml2;

//bool load_ui_from_xml(const std::string& filename, UI_ELEM_LIST& elem_list)
//{
//    XMLDocument doc;
//    if (doc.LoadFile(filename.c_str())) return false;
//
//        // Проверка корневого тега OpenGUI
//        XMLElement* root = doc.RootElement();
//        if (!root || strcmp(root->Name(), "OpenGUI") != 0) return false;
//
//        // Проверка версии и размеров (опционально)
//            const char* version = root->Attribute("version");
//        if (!version || strcmp(version, "0.1") != 0) return false;
//
//        // Парсинг элементов
//            for (XMLElement* elem = root->FirstChildElement(); elem; elem = elem->NextSiblingElement()) {
//                const std::string tag_name = elem->Name();
//                std::unique_ptr<UI_ELEMENT> ui_elem;
//
//                // Обработка <div>
//                if (tag_name == "div") {
//                    auto div = std::make_unique<UI_DIV>();
//                    elem->QueryIntAttribute("width", &div->width);
//                    elem->QueryIntAttribute("height", &div->height);
//                    elem->QueryIntAttribute("x_pos", &div->x_pos);
//                    elem->QueryIntAttribute("y_pos", &div->y_pos);
//                    elem->QueryIntAttribute("z_index", &div->z_index);
//
//                    if (const char* color = elem->Attribute("background_color")) {
//                        div->background_color = hex_to_vec3(color);
//                    }
//
//                    ui_elem = std::move(div);
//                }
//                // Обработка <img>
//                else if (tag_name == "img") {
//                    auto img = std::make_unique<UI_IMG>();
//                    elem->QueryIntAttribute("width", &img->width);
//                    elem->QueryIntAttribute("height", &img->height);
//                    elem->QueryIntAttribute("x_pos", &img->x_pos);
//                    elem->QueryIntAttribute("y_pos", &img->y_pos);
//                    elem->QueryIntAttribute("z_index", &img->z_index);
//
//                    if (const char* src = elem->Attribute("src")) {
//                        img->src = src;
//                    }
//
//                    ui_elem = std::move(img);
//                }
//                // Обработка <font>
//                else if (tag_name == "font") {
//                    auto font = std::make_unique<UI_FONT>();
//                    elem->QueryIntAttribute("x_pos", &font->x_pos);
//                    elem->QueryIntAttribute("y_pos", &font->y_pos);
//                    elem->QueryIntAttribute("z_index", &font->z_index);
//                    elem->QueryIntAttribute("size", &font->pixel_size_px);
//
//                    if (const char* color = elem->Attribute("color")) {
//                        font->color = hex_to_vec3(color);
//                    }
//                    if (const char* font_name = elem->Attribute("font_name")) {
//                        font->font_name = font_name;
//                    }
//                    if (const char* text = elem->Attribute("text")) {
//                        font->text = text;
//                    }
//
//                    ui_elem = std::move(font);
//                }
//
//                // Добавление класса
//                if (ui_elem) {
//                    if (const char* class_name = elem->Attribute("class_name")) {
//                        ui_elem->class_name = class_name;
//                    }
//                    elem_list.ui_elements.push_back(std::move(ui_elem));
//                }
//            }
//
//    return true;
//}

glm::vec3 hex_to_vec3(const std::string& hex) {
    if (hex.size() < 7 || hex[0] != '#') return glm::vec3(0);

    try {
        std::string hex_part = hex.substr(1);
        int rgb = std::stoi(hex_part, nullptr, 16);
        int r = (rgb >> 16) & 0xFF;
        int g = (rgb >> 8) & 0xFF;
        int b = rgb & 0xFF;
        return glm::vec3(r / 255.0f, g / 255.0f, b / 255.0f);
    }
    catch (const std::exception&) {
        return glm::vec3(0);
    }
}

unsigned int load_texture(const char* path)
{
    stbi_set_flip_vertically_on_load(true);
    unsigned int textureID;
    glGenTextures(1, &textureID);
    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}


std::wstring char_to_wstring(const char* str) {
    if (!str) return L"";

    int size_needed = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
    std::wstring wstr(size_needed, 0);
    MultiByteToWideChar(CP_ACP, 0, str, -1, &wstr[0], size_needed);
    return wstr;
}

void open_url(const std::wstring& url) {
    ShellExecuteW(NULL, L"open", url.c_str(), NULL, NULL, SW_SHOWNORMAL);
}
std::string select_file()
{
    IFileOpenDialog* pFileOpen;
    std::string result;
    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
        COINIT_DISABLE_OLE1DDE);
    hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
        IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));
    if (SUCCEEDED(hr))
    {
        // Show the Open dialog box.
        hr = pFileOpen->Show(NULL);

        // Get the file name from the dialog box.
        if (SUCCEEDED(hr))
        {
            IShellItem* pItem;
            hr = pFileOpen->GetResult(&pItem);
            if (SUCCEEDED(hr))
            {
                PWSTR pszFilePath;
                hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                // Display the file name to the user.
                if (SUCCEEDED(hr))
                {
                    result = wstrtostr(pszFilePath);
                    CoTaskMemFree(pszFilePath);
                    /* menu_state->switch_plane_file_loaded_flag();*/
                }
                pItem->Release();
            }
        }
        pFileOpen->Release();
    }
    CoUninitialize();
    return result;
}

std::string select_folder()
{
    IFileOpenDialog* pFileOpen;
    std::string result;
    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
        COINIT_DISABLE_OLE1DDE);
    hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
        IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));
    if (SUCCEEDED(hr))
    {
        // Устанавливаем опцию для выбора папок вместо файлов
        hr = pFileOpen->SetOptions(FOS_PICKFOLDERS);

        // Show the Open dialog box.
        hr = pFileOpen->Show(NULL);

        // Get the folder name from the dialog box.
        if (SUCCEEDED(hr))
        {
            IShellItem* pItem;
            hr = pFileOpen->GetResult(&pItem);
            if (SUCCEEDED(hr))
            {
                PWSTR pszFilePath;
                hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                // Display the folder name to the user.
                if (SUCCEEDED(hr))
                {
                    result = wstrtostr(pszFilePath);
                    CoTaskMemFree(pszFilePath);

                }
                pItem->Release();
            }
        }
        pFileOpen->Release();
    }
    CoUninitialize();
    return result;
}

std::string wstrtostr(const std::wstring& wstr)
{
    std::string strTo;
    char* szTo = new char[wstr.length() + 1];
    szTo[wstr.size()] = '\0';
    WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, szTo, (int)wstr.length(), NULL, NULL);
    strTo = szTo;
    delete[] szTo;
    return strTo;
}