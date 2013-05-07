
#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "object.h"
#include "shader.h"

class Sphere : public Object
{
public:
	Sphere();
	
	bool Initialize(int slices, int stacks, float radius, int shader, int hit, float born_time);

	void hit(float time, float born_time);
	bool is_sphere_hit();
	void reset();

	float getTime();
	void setTime(float t);
	float getBirthTime();
	float getEndTime();
	glm::vec3 getPostion();
	void setPosition(glm::vec3 pos);

	virtual void Draw(const glm::ivec2 & size);
	virtual void Draw(const glm::mat4 & projection, glm::mat4 modelview, const glm::ivec2 & size, GLint level, GLint shader, const float time = 0);
	void TakeDown();
	void StepShader();
	Shader phong;
	Shader blue;
	ADSShader light;
	ADSShader adsShader;
	Shader solid_color;

private:
	void BuildNormalVisualizationGeometry();
	glm::vec4 colors[2];
	std::vector<VertexAttributesPCN> vertices;
	std::vector<Shader> shaders;
	GLuint shader_index;
	typedef Object super;
	bool is_hit;
	float time;
	float born_time;
	float end_time;
	glm::vec3 position;
};
