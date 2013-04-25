
#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "object.h"
#include "shader.h"

class Obstacle : public Object
{
public:
	Obstacle();
	
	bool Initialize(float height, float width, int shader);

	float getTime();
	glm::vec3 getPostion();
	void setPosition(glm::vec3 pos);
	float getForce();
	void setForce(float f);
	glm::vec3 getDirection();
	void setDirection(glm::vec3 dir);

	virtual void Draw(const glm::ivec2 & size);
	virtual void Draw(const glm::mat4 & projection, glm::mat4 modelview, const glm::ivec2 & size, const float time = 0);
	void TakeDown();
	Shader shader;
	ADSShader adsShader;
	Shader solid_color;

private:
	void BuildNormalVisualizationGeometry();
	glm::vec4 colors[2];
	std::vector<VertexAttributesPCNT> vertices;
	typedef Object super;
	//float time;
	glm::vec3 position;
	float force;
	glm::vec3 direction;
};
