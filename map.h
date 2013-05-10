
#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "object.h"
#include "shader.h"
#include "fbo.h"

class Map : public Object
{
public:
	Map();
	
	bool InitializeFloor(bool minimap, bool clock, bool floor);

	bool InitializeWalls();

	bool InitializeCursor();

	virtual void Draw(const glm::ivec2 & size);
	void Draw(const glm::mat4 & projection, glm::mat4 modelview, const glm::ivec2 & size, GLint level, GLint shader, const float time);
	void Draw_walls(const glm::mat4 & projection, glm::mat4 modelview, const glm::ivec2 & size, const float time = 0);
	void Draw_floors(const glm::mat4 & projection, glm::mat4 modelview, const glm::ivec2 & size, FrameBufferObject fbo, const float time);

	void DrawCursor(const glm::mat4 & projection, glm::mat4 modelview, const glm::ivec2 & size);

	void TakeDown();
	Shader shader;
	ADSShader adsShader;
	Shader solid_color;

private:
	void BuildNormalVisualizationGeometry();
	glm::vec4 colors[2];
	std::vector<VertexAttributesPCNT> vertices;
	typedef Object super;
	float time;
};
