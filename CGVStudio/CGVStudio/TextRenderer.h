#pragma once

#include <GL/glew.h>
#include <glfw/glfw3.h>
#include <string>
#include <map>
#include "../thirdparty/glm/glm.hpp"
#include "../thirdparty/glm/gtc/type_ptr.hpp"
#include "../thirdparty/glm/gtc/matrix_transform.hpp"
#include "freeimage.h"
#include "Shader.h"
#include <ft2build.h>
#include FT_FREETYPE_H

// Tutorial for UI elements https://learnopengl.com/In-Practice/Text-Rendering

struct Character {
    unsigned int TextureID;
    glm::ivec2   Size;
    glm::ivec2   Bearing;
    unsigned int Advance;
};

class TextRenderer {
public:
    TextRenderer();
    ~TextRenderer();

    void RenderText(std::string text, float x, float y, float scale, glm::vec3 color);

private:
    std::map<GLchar, Character> Characters;
    unsigned int VAO, VBO;
    Shader shader;

    void initText();
};
