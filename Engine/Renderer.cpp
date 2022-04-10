#include "Renderer.h"
#include "GLFW/glfw3.h"
#include <stdio.h>

#include "OpenGL/VertexArray.h"
#include "OpenGL/Buffer.h"
#include "OpenGL/Shader.h"
#include "OpenGL/Texture.h"
#include <array>

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/string_cast.hpp"

#include <map>


#include <ft2build.h>
#include FT_FREETYPE_H

#include <iostream>
#include <string>
struct Vertex
{
    glm::vec2 Pos;
    glm::vec2 TexCoords;
    float TexID;
};

struct UIVertex
{
    glm::vec2 Pos;
    glm::vec2 TexCoords;
    glm::vec4 Color;
};

struct Character {
    Texture TextureID;  // ID handle of the glyph texture
    glm::ivec2   Size;       // Size of glyph
    glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
    unsigned int Advance;    // Offset to advance to next glyph
};

struct TextVertex
{
    glm::vec2 Pos;
    glm::vec2 TexCoords;
    glm::vec3 Color;
    float TexID;
};

struct RenderingData
{
    VertexArray VA;
    VertexBuffer VB;
    IndexBuffer IB;
    Shader shader;

    uint32_t QuadIndexCount = 0;
    Vertex* Base = nullptr;
    Vertex* Pointer = nullptr;

    static const uint32_t MaxQuads = 1000;
    static const uint32_t MaxVerticies = MaxQuads * 4;
    static const uint32_t MaxIndicies = MaxQuads * 6;

    VertexArray UIVA;
    VertexBuffer UIVB;
    Shader UIshader;

    uint32_t UIIndexCount = 0;
    UIVertex* UIBase = nullptr;
    UIVertex* UIPointer = nullptr;


    static const uint32_t MaxUI = 1000;
    static const uint32_t UIMaxVerticies = MaxQuads * 4;
    static const uint32_t UIMaxIndicies = MaxQuads * 6;

    VertexArray TextVA;
    VertexBuffer TextVB;
    Shader Textshader;

    uint32_t TextIndexCount = 0;
    TextVertex* TextBase = nullptr;
    TextVertex* TextPointer = nullptr;


    static const uint32_t MaxText = 1000;
    static const uint32_t TextMaxVerticies = MaxQuads * 4;
    static const uint32_t TextMaxIndicies = MaxQuads * 6;

    static const uint32_t MaxTextureSlots = 32;

    std::array<Texture, MaxTextureSlots> TextureSlots;
    uint32_t TextureSlotIndex = 0;

    glm::vec4 QuadVertexPositions[4];

};

static RenderingData data;
static std::map<char, Character> Characters;
void Renderer::Initialize()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to initialize OpenGL context");
    }
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    data.VA.Create();
    data.VA.Bind();

    data.VB.Create();
    data.VB.Bind();

    data.shader.Create("shader.shader");
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Pos));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, TexCoords));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, TexID));
    data.Base = new Vertex[data.MaxVerticies];

    uint32_t* QuadIndicies = new uint32_t[data.MaxIndicies];

    uint32_t offset = 0;
    for (int i = 0; i < data.MaxIndicies; i += 6)
    {
        QuadIndicies[i + 0] = offset + 0;
        QuadIndicies[i + 1] = offset + 1;
        QuadIndicies[i + 2] = offset + 2;

        QuadIndicies[i + 3] = offset + 2;
        QuadIndicies[i + 4] = offset + 3;
        QuadIndicies[i + 5] = offset + 0;

        offset += 4;
    }

    IndexBuffer m_IndexBuffer;
    m_IndexBuffer.Create();
    m_IndexBuffer.Bind();
    m_IndexBuffer.UploadData(QuadIndicies, data.MaxIndicies);

    data.UIVA.Create();
    data.UIVA.Bind();

    data.UIVB.Create();
    data.UIVB.Bind();

    data.UIshader.Create("UI.shader");
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(UIVertex), (const void*)offsetof(UIVertex, Pos));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(UIVertex), (const void*)offsetof(UIVertex, TexCoords));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(UIVertex), (const void*)offsetof(UIVertex, Color));
    data.UIBase = new UIVertex[data.UIMaxVerticies];
    m_IndexBuffer.Bind();

    data.TextVA.Create();
    data.TextVA.Bind();

    data.TextVB.Create();
    data.TextVB.Bind();

    data.Textshader.Create("Text.shader");
    data.TextBase = new TextVertex[data.TextMaxVerticies];
    m_IndexBuffer.Bind();
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(TextVertex), (const void*)offsetof(TextVertex, Pos));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(TextVertex), (const void*)offsetof(TextVertex, TexCoords));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(TextVertex), (const void*)offsetof(TextVertex, Color));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(TextVertex), (const void*)offsetof(TextVertex, TexID));
    delete[] QuadIndicies;

   

    int32_t samplers[data.MaxTextureSlots];
    for (int32_t i = 0; i < data.MaxTextureSlots; i++)
    {
        samplers[i] = i;
    }
    glm::mat4 proj = glm::ortho(0.0f, 1600.0f, 900.0f, 0.0f, 0.0f, 1.0f);

    data.shader.Bind();
    data.shader.UploadUniformIV1("u_Textures", samplers, data.MaxTextureSlots);
    data.shader.UploadUniformMat4("proj", proj);
    proj = glm::ortho(0.0f, 1600.0f, 0.0f, 900.0f, 0.0f, 1.0f);

    data.Textshader.Bind();
    data.Textshader.UploadUniformIV1("u_Textures", samplers, data.MaxTextureSlots);
    data.Textshader.UploadUniformMat4("proj", proj);

    data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
    data.QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
    data.QuadVertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
    data.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

    
#pragma region TextInit
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        return;
    }

    FT_Face face;
    if (FT_New_Face(ft, "Times.ttf", 0, &face))
    {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        return;
    }

    FT_Set_Pixel_Sizes(face, 0, 48);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

    for (unsigned char c = 0; c < 128; c++)
    {
        // load character glyph 
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
            // generate texture

            Texture texture;
            texture.Create(face->glyph->bitmap.width, face->glyph->bitmap.rows, face->glyph->bitmap.buffer, GL_RED, GL_UNSIGNED_BYTE);
        
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // set texture options
        
        // now store character for later use
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };
        Characters.insert(std::pair<char, Character>(c, character));
    }


    FT_Done_Face(face);
    FT_Done_FreeType(ft);
#pragma endregion

}

void Renderer::Clear(float r, float g, float b)
{
    glClearColor(r, g, b, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::BeginScene()
{
    data.QuadIndexCount = 0;
    data.Pointer = data.Base;
    data.TextureSlotIndex = 0;

    data.UIIndexCount = 0;
    data.UIPointer = data.UIBase;

    data.TextIndexCount = 0;
    data.TextPointer = data.TextBase;
}
void Renderer::EndScene()
{
    Flush();
}
void Renderer::Flush()
{
    for (uint32_t i = 0; i < data.TextureSlotIndex; i++)
    {
        data.TextureSlots[i].Bind(i);
    }
    if (data.QuadIndexCount)
    {
        data.VA.Bind();
        data.VB.Bind();
        data.shader.Bind();
        uint32_t dataSize = (uint8_t*)data.Pointer - (uint8_t*)data.Base;
        data.VB.UploadData(data.Base, dataSize);
        glDrawElements(GL_TRIANGLES, data.QuadIndexCount, GL_UNSIGNED_INT, 0);
    }

   
    if (data.UIIndexCount)
    {
        data.UIVA.Bind();
        data.UIVB.Bind();
        data.UIshader.Bind();
        uint32_t dataSize = (uint8_t*)data.UIPointer - (uint8_t*)data.UIBase;
        data.UIVB.UploadData(data.UIBase, dataSize);
        
        glDrawElements(GL_TRIANGLES, data.UIIndexCount, GL_UNSIGNED_INT, 0);
    }

    if (data.TextIndexCount)
    {
        data.TextVA.Bind();
        data.TextVB.Bind();
        data.Textshader.Bind();
        uint32_t dataSize = (uint8_t*)data.TextPointer - (uint8_t*)data.TextBase;
        data.TextVB.UploadData(data.TextBase, dataSize);
        glDrawElements(GL_TRIANGLES, data.TextIndexCount, GL_UNSIGNED_INT, 0);
    }
}

void Renderer::NextBatch()
{
    EndScene();

    data.QuadIndexCount = 0;
    data.Pointer = data.Base;
    data.TextureSlotIndex = 0;

    data.UIIndexCount = 0;
    data.UIPointer = data.UIBase;

    data.TextIndexCount = 0;
    data.TextPointer = data.TextBase;
}

void Renderer::SetCamera(const glm::vec2& Position)
{
    data.shader.Bind();
    data.shader.UploadUniformF2("Camera", Position.x, Position.y);

}

void Renderer::DrawQuad(const glm::mat4& transform, const Texture& texture, const glm::vec2& texCoordsMin, const glm::vec2& texCoordsMax)
{
    if (data.QuadIndexCount >= data.MaxIndicies)
    {
        NextBatch();
    }

    float textureIndex = 0.0f;

    for (uint32_t i = 1; i < data.TextureSlotIndex; i++)
    {
        if (data.TextureSlots[i].GetID() == texture.GetID())
        {
            textureIndex = (float)i;
            break;
        }
    }

    if (textureIndex == 0.0f)
    {
        textureIndex = (float)data.TextureSlotIndex;
        data.TextureSlots[data.TextureSlotIndex] = texture;
        data.TextureSlotIndex++;
    }

    data.Pointer->Pos = transform * data.QuadVertexPositions[0];
    data.Pointer->TexCoords = { texCoordsMin.x, texCoordsMin.y };
    data.Pointer->TexID = textureIndex;
    data.Pointer++;

    data.Pointer->Pos = transform * data.QuadVertexPositions[1];
    data.Pointer->TexCoords = { texCoordsMax.x, texCoordsMin.y };
    data.Pointer->TexID = textureIndex;
    data.Pointer++;

    data.Pointer->Pos = transform * data.QuadVertexPositions[2];
    data.Pointer->TexCoords = { texCoordsMax.x, texCoordsMax.y };
    data.Pointer->TexID = textureIndex;
    data.Pointer++;

    data.Pointer->Pos = transform * data.QuadVertexPositions[3];
    data.Pointer->TexCoords = { texCoordsMin.x, texCoordsMax.y };
    data.Pointer->TexID = textureIndex;
    data.Pointer++;

    data.QuadIndexCount += 6;
}

void Renderer::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Texture& texture)
{
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(position.x, position.y, 0.0)) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
    DrawQuad(transform, texture);
}

auto rotAroundPoint(float rad, const glm::vec3& point)
{
    auto t1 = glm::translate(glm::mat4(1), -point);
    auto r = glm::rotate(glm::mat4(1), rad, {0, 0, 1});
    auto t2 = glm::translate(glm::mat4(1), point);
    return t2 * r * t1;
}


void Renderer::DrawQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const Texture& texture)
{
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(position.x, position.y, 0.0)) * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), {0, 0, 1}) *glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
    DrawQuad(transform, texture);
}

void Renderer::DrawQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const Texture& texture, int CurrentFrame, int MaxFrames)
{
    glm::vec2 min = { ((float)CurrentFrame) / MaxFrames, 0.0f };
    glm::vec2 max = { ((float)CurrentFrame + 1) / MaxFrames, 1.0f };
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(position.x, position.y, 0.0)) * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0, 0, 1 }) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
    DrawQuad(transform, texture, min, max);
}

void Renderer::DrawQuad(const glm::vec2& position, float rotation, glm::vec2 rotationPoint, const glm::vec2& size, const Texture& texture)
{
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(position.x, position.y, 0.0)) * rotAroundPoint(glm::radians(rotation), { rotationPoint.x, rotationPoint.y, 0 }) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
    DrawQuad(transform, texture);
}

void Renderer::DrawUI(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
{
    if (data.UIIndexCount >= data.UIMaxIndicies)
    {
        NextBatch();
    }
    data.UIPointer->Pos = {position.x, position.y};
    data.UIPointer->TexCoords = { 0, 0 };
    data.UIPointer->Color = color;
    data.UIPointer++;

    data.UIPointer->Pos = { position.x + size.x, position.y };
    data.UIPointer->TexCoords = { 1, 0 };
    data.UIPointer->Color = color;
    data.UIPointer++;

    data.UIPointer->Pos = { position.x + size.x, position.y + size.y };
    data.UIPointer->TexCoords = { 1, 1 };
    data.UIPointer->Color = color;
    data.UIPointer++;

    data.UIPointer->Pos = { position.x , position.y + size.y };
    data.UIPointer->TexCoords = { 0, 1 };
    data.UIPointer->Color = color;
    data.UIPointer++;

    data.UIIndexCount += 6;
}



void Renderer::DrawText(std::string text, const glm::vec2& position, float scale, const glm::vec3& color)
{
    glm::vec2 pos = position;

    // iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = Characters[*c];

        if (data.QuadIndexCount >= data.MaxIndicies)
        {
            NextBatch();
        }

        float textureIndex = 0.0f;

        for (uint32_t i = 1; i < data.TextureSlotIndex; i++)
        {
            if (data.TextureSlots[i].GetID() == ch.TextureID.GetID())
            {
                textureIndex = (float)i;
                break;
            }
        }

        if (textureIndex == 0.0f)
        {
            textureIndex = (float)data.TextureSlotIndex;
            data.TextureSlots[data.TextureSlotIndex] = ch.TextureID;
            data.TextureSlotIndex++;
        }

        float xpos = pos.x + ch.Bearing.x * scale;
        float ypos = pos.y - (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;
        // update VBO for each character

        data.TextPointer->Pos = { xpos, ypos };
        data.TextPointer->TexCoords = { 0, 1 };
        data.TextPointer->Color = color;
        data.TextPointer->TexID = textureIndex;
        data.TextPointer++;

        data.TextPointer->Pos = { xpos + w, ypos };
        data.TextPointer->TexCoords = { 1, 1 };
        data.TextPointer->Color = color;
        data.TextPointer->TexID = textureIndex;
        data.TextPointer++;

        data.TextPointer->Pos = { xpos + w, ypos + h };
        data.TextPointer->TexCoords = { 1, 0 };
        data.TextPointer->Color = color;
        data.TextPointer->TexID = textureIndex;
        data.TextPointer++;

        data.TextPointer->Pos = { xpos, ypos + h };
        data.TextPointer->TexCoords = { 0, 0 };
        data.TextPointer->Color = color;
        data.TextPointer->TexID = textureIndex;
        data.TextPointer++;

        data.TextIndexCount += 6;

        // render glyph texture over quad
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        pos.x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
    }
}