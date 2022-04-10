#pragma once
#include "glm/glm.hpp"
#include "OpenGL/Texture.h"
#include <string>
class Renderer
{
public:
	static void Initialize();

	static void Clear(float r, float g, float b);

	static void BeginScene();
	static void EndScene();
	static void Flush();
	static void NextBatch();

	static void SetCamera(const glm::vec2& Position);

	static void DrawQuad(const glm::mat4& transform, const Texture& texture, const glm::vec2& texCoordsMin = { 0, 0 }, const glm::vec2& texCoordsMax = { 1, 1 });
	static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Texture& texture);
	static void DrawQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const Texture& texture);
	static void DrawQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const Texture& texture, int CurrentFrame, int MaxFrames);
	static void DrawQuad(const glm::vec2& position, float rotation, glm::vec2 rotationPoint, const glm::vec2& size, const Texture& texture);

	static void DrawUI(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);

	static void DrawText(std::string text, const glm::vec2& position, float scale, const glm::vec3& color);

};