#include "shader.h"
#include <assert.h>

using namespace std;
using namespace glm;

#define BAD_GL_VALUE GLuint(-1)

Shader::Shader()
{
	this->vertex_shader_id = BAD_GL_VALUE;
	this->fragment_shader_id = BAD_GL_VALUE;
	this->program_id = BAD_GL_VALUE;
	this->modelview_matrix_handle = BAD_GL_VALUE;
	this->projection_matrix_handle = BAD_GL_VALUE;
	this->normal_matrix_handle = BAD_GL_VALUE;
	this->size_handle = BAD_GL_VALUE;
}

/*	This Shader() class implements or assumes a basic set of uniforms will be
	provided to all shaders derived from it. These are listed below. 
	Shader::CommonSetup() can be used by call derived classes to send the 
	common values to the shader. Values unique to the derived class can be
	loaded with the CustomShader() function.
*/

void Shader::CommonSetup(float time, const GLint * size, const GLfloat * projection, const GLfloat * modelview, const GLfloat * mvp, const GLfloat * nm)
{
	glUniform1f(this->time_handle, time);
	glUniform2iv(this->size_handle, 1, size);
	glUniformMatrix4fv(this->projection_matrix_handle, 1, GL_FALSE, projection);
	glUniformMatrix4fv(this->modelview_matrix_handle, 1, GL_FALSE, modelview);
	glUniformMatrix4fv(this->mvp_handle, 1, GL_FALSE, mvp);
	glUniformMatrix3fv(this->normal_matrix_handle, 1, GL_FALSE, nm);
}

void Shader::printShader()
{
	GLint nUniforms;
	glGetProgramiv(this->program_id, GL_ACTIVE_UNIFORMS, &nUniforms);
	cout << "nUniforms: " << nUniforms << endl;
}

void Shader::Use()
{
	assert(this->program_id != BAD_GL_VALUE);
	glUseProgram(this->program_id);
}

/*	The shader initialization code is lifted liberally from the GLSL 4.0 Cookbook.
*/

bool Shader::Initialize(char * vertex_shader_file, char * fragment_shader_file)
{
	GLint check_value;

	if (GLReturnedError("Shader::Initialize - on entrance"))
		return false;

	this->vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
	this->LoadShader(vertex_shader_file, this->vertex_shader_id);
	glCompileShader(this->vertex_shader_id);
	glGetShaderiv(this->vertex_shader_id, GL_COMPILE_STATUS, &check_value);
	if (check_value != GL_TRUE)
	{
		cerr << this->GetShaderLog(vertex_shader_id).str();
		cerr << "GLSL compilation failed - vertex shader: " << vertex_shader_file << endl;
		return false;
	}

	if (GLReturnedError("Shader::Initialize - after processing vertex shader"))
		return false;

	this->fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
	this->LoadShader(fragment_shader_file, this->fragment_shader_id);
	glCompileShader(this->fragment_shader_id);
	glGetShaderiv(this->fragment_shader_id, GL_COMPILE_STATUS, &check_value);
	if (check_value != GL_TRUE)
	{
		cerr << this->GetShaderLog(fragment_shader_id).str();
		cerr << "GLSL compilation failed - fragment shader: " << fragment_shader_file << endl;
		return false;
	}

	this->program_id = glCreateProgram();
	glAttachShader(this->program_id, this->vertex_shader_id);
	glAttachShader(this->program_id, this->fragment_shader_id);
	glLinkProgram(program_id);

	glDeleteShader(vertex_shader_id);
	glDeleteShader(fragment_shader_id);

	glUseProgram(this->program_id);

	this->modelview_matrix_handle = glGetUniformLocation(this->program_id, (const GLchar *) "modelview_matrix");
	this->projection_matrix_handle = glGetUniformLocation(this->program_id, (const GLchar *) "projection_matrix");
	this->normal_matrix_handle = glGetUniformLocation(this->program_id, (const GLchar *) "normal_matrix");
	this->mvp_handle = glGetUniformLocation(this->program_id, (const GLchar *) "mvp");
	this->size_handle = glGetUniformLocation(this->program_id, (const GLchar *) "size");
	this->time_handle = glGetUniformLocation(this->program_id, (const GLchar *) "time");

	glUseProgram(0);

	return !GLReturnedError("Shader::Initialize - on exit");
}

void Shader::CustomSetup()
{
}


void Shader::TakeDown()
{
	GLint temp;
	GLsizei size;

	if (this->program_id == (GLuint) -1)
		return;

	glGetProgramiv(this->program_id, GL_ATTACHED_SHADERS, &temp);
	if (temp > 0)
	{
		GLuint * shader_list = new GLuint[temp];
		glGetAttachedShaders(this->program_id, temp, &size, shader_list);
		for (GLsizei i = 0; i < size; i++)
		{
			glDetachShader(this->program_id, shader_list[i]);
			// The shaders were marked for deletion
			// immediately after they were created.
		}
		delete [] shader_list;
	}

	glDeleteProgram(this->program_id);
	this->program_id = (GLuint) -1;
}

/*
	This function is adapted from OpenGL 4.0 Shading Language Cookbook by David Wolff.
*/

bool Shader::LoadShader(const char * file_name, GLuint shader_id)
{
	assert(file_name != NULL);
	if (GLReturnedError("Shader::LoadShader - on entrance"))
		return false;

	FILE * file_handle = NULL;
	fopen_s(&file_handle, file_name, "rb");
	if (file_handle == NULL)
	{
		cerr <<  "Cannot open shader: " << file_name << endl;
		return false;
	}
	fseek(file_handle, 0, SEEK_END);
	int length = ftell(file_handle);
	fseek(file_handle, 0, SEEK_SET);
	GLubyte * buffer = new GLubyte[length + 1];
	fread(buffer, sizeof(GLubyte), length, file_handle);
	fclose(file_handle);
	buffer[length] = 0;
	glShaderSource(shader_id, 1, (const char **) &buffer, NULL);
	delete [] buffer;

	return !GLReturnedError("Shader::LoadShader - on exit");
}

/*
	This function is adapted from OpenGL 4.0 Shading Language Cookbook by David Wolff.
*/

stringstream Shader::GetShaderLog(GLuint shader_id)
{
	stringstream s;
	GLint log_length;
	glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_length);
	if (log_length <= 0)
		s << "No shader log information available." << endl;
	else
	{
		GLchar * buffer = new GLchar[log_length];
		glGetShaderInfoLog(shader_id, log_length, NULL, buffer);
		s << "Shader log:" << endl;
		s << buffer << endl;
		delete [] buffer;
	}
	return s;
}

bool Shader::GLReturnedError(char * s)
{
	bool return_error = false;
	GLenum glerror;

	while ((glerror = glGetError()) != GL_NO_ERROR)
	{
		return_error = true;
		cerr << s << ": " << gluErrorString(glerror) << endl;
	}

	return return_error;
}

BackgroundShader::BackgroundShader() : super()
{
	this->color_array_handle = BAD_GL_VALUE;
}

bool BackgroundShader::Initialize(char * vertex_shader_file, char * fragment_shader_file)
{
	if (!super::Initialize(vertex_shader_file, fragment_shader_file))
		return false;

	this->Use();
	this->color_array_handle = glGetUniformLocation(this->program_id, (const GLchar *) "color_array");
	glUseProgram(0);
	assert(this->color_array_handle != BAD_GL_VALUE);
	return true;
}

void BackgroundShader::CustomSetup(vec4 * color_array)
{
	glUniform4fv(this->color_array_handle, 4, (GLfloat *) color_array);
}

ADSShader::ADSShader() : super()
{
	this->position_handle = BAD_GL_VALUE;
	this->la_handle = BAD_GL_VALUE;
	this->ld_handle = BAD_GL_VALUE;
	this->ls_handle = BAD_GL_VALUE;
	this->ka_handle = BAD_GL_VALUE;
	this->kd_handle = BAD_GL_VALUE;
	this->ks_handle = BAD_GL_VALUE;
	this->shininess_handle = BAD_GL_VALUE;
}

bool ADSShader::Initialize(char * vertex_shader_file, char * fragment_shader_file)
{
	if (!super::Initialize(vertex_shader_file, fragment_shader_file))
		return false;

	this->Use();	

	this->position_handle = glGetUniformLocation(this->program_id, (const GLchar *) "light.position");	
	this->ka_handle = glGetUniformLocation(this->program_id, (const GLchar *) "material.ka");
	this->kd_handle = glGetUniformLocation(this->program_id, (const GLchar *) "material.kd");
	this->ks_handle = glGetUniformLocation(this->program_id, (const GLchar *) "material.ks");
	this->la_handle = glGetUniformLocation(this->program_id, (const GLchar *) "light.la");
	this->ld_handle = glGetUniformLocation(this->program_id, (const GLchar *) "light.ld");
	this->ls_handle = glGetUniformLocation(this->program_id, (const GLchar *) "light.ls");
	this->shininess_handle = glGetUniformLocation(this->program_id, (const GLchar *) "material.shininess");
		
	glUseProgram(0);
	assert(this->position_handle != BAD_GL_VALUE);
	assert(this->ka_handle != BAD_GL_VALUE);
	assert(this->kd_handle != BAD_GL_VALUE);
	assert(this->ks_handle != BAD_GL_VALUE);
	assert(this->la_handle != BAD_GL_VALUE);
	assert(this->ld_handle != BAD_GL_VALUE);
	assert(this->ls_handle != BAD_GL_VALUE);
	assert(this->shininess_handle != BAD_GL_VALUE);
	return true;
}

//void ADSShader::CustomSetup()
//{
//	glUniform4fv(this->color_array_handle, 4, (GLfloat *) color_array);
//}

void ADSShader::SetLight(vec4 position, vec3 la, vec3 ld, vec3 ls)
{
	glUniform4fv(this->position_handle, 1, (const GLfloat *) &position);
	glUniform3fv(this->la_handle, 1, (const GLfloat *) &la);
	glUniform3fv(this->ld_handle, 1, (const GLfloat *) &ld);
	glUniform3fv(this->ls_handle, 1, (const GLfloat *) &ls);
}

void ADSShader::SetMaterial(vec3 ka, vec3 kd, vec3 ks, float shininess)
{
	glUniform3fv(this->ka_handle, 1, (const GLfloat *) &ka);
	glUniform3fv(this->kd_handle, 1, (const GLfloat *) &kd);
	glUniform3fv(this->ks_handle, 1, (const GLfloat *) &ks);
	glUniform1f(this->shininess_handle, shininess);
}

TexturedShader::TexturedShader() : super()
{
	this->texture_handle = GLuint(-1);
}

bool TexturedShader::Initialize(char * vertex_shader_file, char * fragment_shader_file)
{
	if (super::Initialize(vertex_shader_file, fragment_shader_file) == false)
		return false;

	this->Use();
	this->texture_handle = glGetUniformLocation(this->program_id, (const GLchar *) "picture");
	glUseProgram(0);
	return true;
}

void TexturedShader::CustomSetup()
{
	glUniform1i(this->texture_handle, 0);
}

//Text shader
//TextShader::TextShader() : super()
//{
//
//}
//
//bool TextShader::Initialize(char * vertex_shader_file, char * fragment_shader_file)
//{
//	/* Initialize the FreeType2 library */
//	if (FT_Init_FreeType(&ft)) {
//		fprintf(stderr, "Could not init freetype library\n");
//		return 0;
//	}
//
//	/* Load a font */	
//	if (FT_New_Face(ft, fontfilename, 0, &face)) {
//		fprintf(stderr, "Could not open font %s\n", fontfilename);
//		return 0;
//	}
//
//	if (super::Initialize(vertex_shader_file, fragment_shader_file) == false)
//		return false;
//
//
//	attribute_coord = get_attrib(program, "coord");	
//	uniform_tex = get_uniform(program, "tex");
//
//	uniform_color = get_uniform(program, "color");
//		
//	if(attribute_coord == -1 || uniform_tex == -1 || uniform_color == -1)
//		return 0;
//
//	// Create the vertex buffer object	
//	glGenBuffers(1, &vbo);
//
//	return 1;	
//
//}

ToonShader::ToonShader() : super()
{  
  this->position_handle = BAD_GL_VALUE;  
  this->intensity_handle = BAD_GL_VALUE;
  this->ka_handle = BAD_GL_VALUE;
  this->kd_handle = BAD_GL_VALUE;  
}

bool ToonShader::Initialize(char * vertex_shader_file, char * fragment_shader_file)
{
  if (!super::Initialize(vertex_shader_file, fragment_shader_file))
    return false;

  this->Use();	

  this->position_handle = glGetUniformLocation(this->program_id, (const char *) "light.position");    
  this->intensity_handle = glGetUniformLocation(this->program_id, (const char *) "light.intensity");
  this->ka_handle = glGetUniformLocation(this->program_id, (const GLchar *) "material.ka");
  this->kd_handle = glGetUniformLocation(this->program_id, (const GLchar *) "material.kd");
  
  glUseProgram(0);

  assert(this->position_handle != BAD_GL_VALUE);    
  assert(this->intensity_handle != BAD_GL_VALUE);
  assert(this->ka_handle != BAD_GL_VALUE);
  assert(this->kd_handle != BAD_GL_VALUE);
  return true;
}

void ToonShader::SetLight(vec4 position, vec3 intensity)
{
  glUniform4fv(this->position_handle, 1, (const GLfloat *) &position);    
  glUniform3fv(this->intensity_handle, 1, (const GLfloat *) &intensity);        
}

void ToonShader::SetMaterial(vec3 ka, vec3 kd)
{
  glUniform3fv(this->ka_handle, 1, (const GLfloat *) &ka);
  glUniform3fv(this->kd_handle, 1, (const GLfloat *) &kd);  
}
