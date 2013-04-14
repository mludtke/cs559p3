#pragma once
#include "shader.h"

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
