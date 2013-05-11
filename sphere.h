#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//#include <glm/gtx/string_cast.hpp>
#include <random>
#include <functional>
#include "object.h"
#include "shader.h"
#include "constants.h"

static std::default_random_engine generator;
static std::uniform_real_distribution<GLfloat> uniform_distribution(-5.0, 5.0);
static auto randomLightPosition = bind(uniform_distribution, generator);

class Sphere : public Object
{
public:
	Sphere();
	
	bool Initialize( int hit, GLfloat radius, GLint slices = 30, GLint stacks = 30);

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
	void StepMaterial();
	void StepLightPos();
	void incX(); void incY(); void incZ();
	void decX(); void decY(); void decZ();
	Shader shader, phong, normal, dark, gooch;		
	ADSShader phong2, twoside, flat, perforated, time_perforated;
	TexturedShader texture;	

private:
	void BuildNormalVisualizationGeometry();	
	std::vector<VertexAttributesPCNT> vertices;
	std::vector<Shader *> shaders;
	GLuint shader_index, material_index, light_index;
	typedef Object super;
	int is_hit;
	GLfloat time, x,y,z, born_time, end_time;
	glm::vec3 position;
	glm::vec4 lightPos[10];
};
