/*	
*/

#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "object.h"
#include "shader.h"

class Skybox : public Object
{
public:
	Skybox();
	bool Initialize();
	virtual void Draw(const glm::ivec2 & size);
	void Draw(const glm::mat4 & projection, glm::mat4 modelview, const glm::ivec2 & size, GLint texture, GLint shader, const float time = 0);
	void TakeDown();
	TexturedShader shader;
	Shader solid_color;  

private:
	void BuildNormalVisualizationGeometry();	
	std::vector<VertexAttributesPCNT> vertices;
	typedef Object super;
};
