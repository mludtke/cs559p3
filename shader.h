#pragma once
#include <iostream>
#include <sstream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
	Shader();
	void TakeDown();
	void Use();
	virtual bool Initialize(char * vertex_shader_file, char * fragment_shader_file);
	virtual void CustomSetup();
	void CommonSetup(float time, const GLint * size, const GLfloat * projection, const GLfloat * modelview, const GLfloat * mvp, const GLfloat * nm);

	GLuint modelview_matrix_handle;
	GLuint projection_matrix_handle;
	GLuint normal_matrix_handle;
	GLuint mvp_handle;
	GLuint size_handle;
	GLuint time_handle;

	GLuint vertex_shader_id;
	GLuint fragment_shader_id;
	GLuint program_id;
	bool LoadShader(const char * file_name, GLuint shader_id);
	std::stringstream GetShaderLog(GLuint shader_id);
	void printShader();

protected:
	bool GLReturnedError(char * caller);

};

class BackgroundShader : public Shader
{
public:
	BackgroundShader();
	virtual bool Initialize(char * vertex_shader_file, char * fragment_shader_file);
	virtual void CustomSetup(glm::vec4 * color_array);

protected:
	GLuint color_array_handle;

private:
	typedef Shader super;
};

class ADSShader : public Shader
{
public:
	ADSShader();
	virtual bool Initialize(char * vertex_shader_file, char * fragment_shader_file);	
	void SetLight(glm::vec4 position, glm::vec3 la, glm::vec3 ld, glm::vec3 ls);
	void SetMaterial(glm::vec3 ka, glm::vec3 kd, glm::vec3 ks, float shininess);

protected:
	GLuint position_handle;
	GLuint la_handle;
	GLuint ld_handle;
	GLuint ls_handle;
	GLuint ka_handle;
	GLuint kd_handle;
	GLuint ks_handle;
	GLuint shininess_handle;

private:
	typedef Shader super;
};


class TexturedShader : public Shader
{
public:
	TexturedShader();
	virtual bool Initialize(char * vertex_shader_file, char * fragment_shader_file);
	virtual void CustomSetup();

private:
	typedef Shader super;
	GLuint texture_handle;
};

class TextShader : public Shader
{
public:
	TextShader();
	virtual bool Initialize(char * vertex_shader_file, char * fragment_shader_file);
private:
	typedef Shader super;
protected:
	GLint attribute_coord;
	GLint uniform_tex;
	GLint uniform_color;
};

class ToonShader : public Shader
{
  public:  
  ToonShader();
  virtual bool Initialize(char * vertex_shader_file, char * fragment_shader_file);	
  void SetLight(glm::vec4 position, glm::vec3 intensity);
  void SetMaterial(glm::vec3 ka, glm::vec3 kd);

protected:  
  GLuint position_handle; 
  GLuint intensity_handle;  
  GLuint ka_handle;
  GLuint kd_handle;  

private:
  typedef Shader super;
};
