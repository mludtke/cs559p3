#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "object.h"
#include "shader.h"

class Text : public Object
{
public:
	Text();
	bool Initialize();
	virtual void Draw(const glm::ivec2 & size);
	void Draw(const glm::mat4 & projection, glm::mat4 modelview, const glm::ivec2 & size, GLint texture, const float time = 0);
	void TakeDown();
	TexturedShader shader;
	Shader solid_color;  

	void render_text(const char *text, float x, float y, float sx, float sy);
	void display();
	int init_resources();

	const char *fontfilename;

private:
	void BuildNormalVisualizationGeometry();	
	std::vector<VertexAttributesPCNT> vertices;
	typedef Object super;
};