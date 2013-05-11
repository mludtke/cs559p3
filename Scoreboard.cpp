/*************************************************************	
*	Scoreboard
*
*	A scoreboard object, similar to the Jumbotron
**************************************************************/

#include <iostream>
#include "Scoreboard.h"

using namespace std;
using namespace glm;

Scoreboard::Scoreboard() : Object()
{
	vec4 lighter_color(MakeColor(255, 69, 0, 1.0f));
	vec4 darker_color = vec4(vec3(lighter_color) /* 2.0f / 3.0f*/, 1.0f);
	this->colors[0] = lighter_color;
	this->colors[1] = lighter_color;
}


void Scoreboard::BuildNormalVisualizationGeometry()
{
	const float normal_scalar = 0.125f;
	for (int j = 1; j <= 3; ++j)
	{
		this->normal_vertices.push_back(VertexAttributesP(this->vertices[this->vertices.size() - j].position));
		this->normal_vertices.push_back(VertexAttributesP(this->vertices[this->vertices.size() - j].position + this->vertices[this->vertices.size() - j].normal * normal_scalar));
		this->normal_indices.push_back(this->normal_vertices.size() - 2);
		this->normal_indices.push_back(this->normal_vertices.size() - 1);
	}
}


bool Scoreboard::Initialize()
{
	if (this->GLReturnedError("Scoreboard::Initialize - on entry"))
		return false;

	if (!super::Initialize())
		return false;

	mat4 m;
	int slices = 20;
	int stacks = 6;
	const vec3 n = normalize(vec3(1.0f, 1.0f, 1.0f));
	const float radius = 0.2f;
	float theta = 0.0f, phi = 0.0f;
	float x, y, z;
	y = 0.0f;
	vec4 location;
	vec3 color = CYAN;
	vec3 color2 = WHITE;
	float height = 5.0f;

	const float increment_slices = (2 * 3.14159f) / (float)slices;
	float stack_increment = height/(float)stacks;

	VertexAttributesPCNT bottomLeft, bottomRight, topLeft, topRight;

	for (int i = 0; i <= stacks; i++)
	{
		for (int j = 0; j < slices; j++)
		{
			x = radius * cos(phi);
			//y = height - (height - (i * stack_increment));
			z = radius * sin(phi);
			location = vec4(x, y, z, 1.0f);

			bottomLeft.position = vec3(location * m);
			bottomLeft.color = color;
			bottomLeft.normal = vec3(normalize(location * m));

			phi += increment_slices;
			x = radius * cos(phi);
			z = radius * sin(phi);
			location = vec4(x, y, z, 1.0f);

			bottomRight.position = vec3(location * m);
			bottomRight.color = color;
			bottomRight.normal = vec3(normalize(location * m));

			y = y + stack_increment;
			location = vec4(x, y, z, 1.0f);

			topRight.position = vec3(location * m);
			topRight.color = color;
			topRight.normal = vec3(normalize(location * m));

			phi -= increment_slices;
			x = radius * cos(phi);
			z = radius * sin(phi);
			location = vec4(x, y, z, 1.0f);

			topLeft.position = vec3(location * m);
			topLeft.color = color;
			topLeft.normal = vec3(normalize(location * m));

			this->vertices.push_back(bottomLeft);
			this->vertices.push_back(bottomRight);
			this->vertices.push_back(topRight);
			this->vertices.push_back(topLeft);

			this->vertex_indices.push_back(vertices.size() - 1);
			this->vertex_indices.push_back(vertices.size() - 3);
			this->vertex_indices.push_back(vertices.size() - 4);
			
			this->vertex_indices.push_back(vertices.size() - 1);
			this->vertex_indices.push_back(vertices.size() - 2);
			this->vertex_indices.push_back(vertices.size() - 3);

			phi += increment_slices;
			y -= stack_increment;
		}
		y += stack_increment;
	}

	bottomLeft.position = vec3(-3.0f, 5.0f, 0.21f);
	bottomLeft.color = color2;
	bottomLeft.normal = vec3(0.0f, 0.0f, -1.0f);

	bottomRight.position = vec3(3.0f, 5.0f, 0.21f);
	bottomRight.color = color2;
	bottomRight.normal = vec3(0.0f, 0.0f, -1.0f);

	topRight.position = vec3(3.0f, 7.0f, 0.21f);
	topRight.color = color2;
	topRight.normal = vec3(0.0f, 0.0f, -1.0f);

	topLeft.position = vec3(-3.0f, 7.0f, 0.21f);
	topLeft.color = color2;
	topLeft.normal = vec3(0.0f, 0.0f, -1.0f);

	this->vertices.push_back(bottomLeft);
	this->vertices.push_back(bottomRight);
	this->vertices.push_back(topRight);
	this->vertices.push_back(topLeft);

	this->vertex_indices.push_back(vertices.size() - 1);
	this->vertex_indices.push_back(vertices.size() - 3);
	this->vertex_indices.push_back(vertices.size() - 4);
			
	this->vertex_indices.push_back(vertices.size() - 1);
	this->vertex_indices.push_back(vertices.size() - 2);
	this->vertex_indices.push_back(vertices.size() - 3);



	if (!this->PostGLInitialize(&this->vertex_array_handle, &this->vertex_coordinate_handle, this->vertices.size() * sizeof(VertexAttributesPCN), &this->vertices[0]))
		return false;

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttributesPCNT), (GLvoid *) 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttributesPCNT), (GLvoid *) (sizeof(vec3)));	
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttributesPCNT), (GLvoid *) (sizeof(vec3) * 2));
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(VertexAttributesPCNT), (GLvoid *) (sizeof(vec3) * 3));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	if (this->normal_vertices.size() > 0)
	{
		if (!this->PostGLInitialize(&this->normal_array_handle, &this->normal_coordinate_handle, this->normal_vertices.size() * sizeof(VertexAttributesP), &this->normal_vertices[0]))
			return false;

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttributesP), (GLvoid *) 0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	
	if (!this->shader.Initialize("phong_shader.vert", "phong_shader.frag"))
			return false;
	
	/*if(shader == 1)
	{
		if (!this->shader.Initialize("gouraud_shader.vert", "gouraud_shader.frag"))
			return false;
	}
	if(shader == 2)
	{
		if(!this->adsShader.Initialize("light.vert", "light.frag"))
			return false;
		//if (!this->shader.Initialize("flat_shader.vert", "flat_shader.frag"))
			//return false;
	}
	if(shader == 3)
	{
		if (!this->adsShader.Initialize("flat_shader.vert", "flat_shader.frag"))
			return false;
	}

	if (!this->solid_color.Initialize("solid_shader.vert", "solid_shader.frag"))
			return false;
*/
	if (this->GLReturnedError("Scoreboard::Initialize - on exit"))
		return false;

	return true;
}

bool Scoreboard::InitializeScreen()
{
	if (!super::Initialize())
		return false;

	vec3 color2 = WHITE;

	VertexAttributesPCNT bottomLeft, bottomRight, topLeft, topRight;

	bottomLeft.texture_coordinate = vec2(0.0f, 0.25f);
	bottomRight.texture_coordinate = vec2(1.0f, 0.25f);
	topLeft.texture_coordinate = vec2(0.0f, 0.75f);
	topRight.texture_coordinate = vec2(1.0f, 0.75f);
	

	bottomLeft.position = vec3(-3.0f, 5.0f, 0.21f);
	bottomLeft.color = color2;
	bottomLeft.normal = vec3(0.0f, 0.0f, -1.0f);

	bottomRight.position = vec3(3.0f, 5.0f, 0.21f);
	bottomRight.color = color2;
	bottomRight.normal = vec3(0.0f, 0.0f, -1.0f);

	topRight.position = vec3(3.0f, 7.0f, 0.21f);
	topRight.color = color2;
	topRight.normal = vec3(0.0f, 0.0f, -1.0f);

	topLeft.position = vec3(-3.0f, 7.0f, 0.21f);
	topLeft.color = color2;
	topLeft.normal = vec3(0.0f, 0.0f, -1.0f);

	this->vertices.push_back(bottomLeft);
	this->vertices.push_back(bottomRight);
	this->vertices.push_back(topRight);
	this->vertices.push_back(topLeft);

	this->vertex_indices.push_back(vertices.size() - 1);
	this->vertex_indices.push_back(vertices.size() - 3);
	this->vertex_indices.push_back(vertices.size() - 4);
			
	this->vertex_indices.push_back(vertices.size() - 1);
	this->vertex_indices.push_back(vertices.size() - 2);
	this->vertex_indices.push_back(vertices.size() - 3);



	if (!this->PostGLInitialize(&this->vertex_array_handle, &this->vertex_coordinate_handle, this->vertices.size() * sizeof(VertexAttributesPCN), &this->vertices[0]))
		return false;

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttributesPCNT), (GLvoid *) 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttributesPCNT), (GLvoid *) (sizeof(vec3)));	
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttributesPCNT), (GLvoid *) (sizeof(vec3) * 2));
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(VertexAttributesPCNT), (GLvoid *) (sizeof(vec3) * 3));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	if (this->normal_vertices.size() > 0)
	{
		if (!this->PostGLInitialize(&this->normal_array_handle, &this->normal_coordinate_handle, this->normal_vertices.size() * sizeof(VertexAttributesP), &this->normal_vertices[0]))
			return false;

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttributesP), (GLvoid *) 0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	
	if (!this->shader.Initialize("phong_shader.vert", "phong_shader.frag"))
			return false;
	

	if (this->GLReturnedError("Scoreboard::Initialize - on exit"))
		return false;

	return true;
}

void Scoreboard::TakeDown()
{
	this->vertices.clear();
	this->shader.TakeDown();
	this->adsShader.TakeDown();
	this->solid_color.TakeDown();
	super::TakeDown();
}

void Scoreboard::Draw(const ivec2 & size)
{
	assert(false);
}

void Scoreboard::Draw(const mat4 & projection, mat4 modelview, const ivec2 & size, GLint level, GLint shade, const float time)
{
	if (this->GLReturnedError("Scoreboard::Draw - on entry"))
		return;

	glEnable(GL_DEPTH_TEST);

	mat4 mvp = projection * modelview;
	mat3 nm = inverse(transpose(mat3(modelview)));

	shader.Use();
	shader.CommonSetup(time, value_ptr(size), value_ptr(projection), value_ptr(modelview), value_ptr(mvp), value_ptr(nm));

	glBindVertexArray(this->vertex_array_handle);
	glDrawElements(GL_TRIANGLES , this->vertex_indices.size(), GL_UNSIGNED_INT , &this->vertex_indices[0]);
	glBindVertexArray(0);
	glUseProgram(0);

	if (this->draw_normals)
	{
		this->solid_color.Use();
		this->solid_color.CommonSetup(time, value_ptr(size), value_ptr(projection), value_ptr(modelview), value_ptr(mvp), value_ptr(nm));
		glBindVertexArray(this->normal_array_handle);
		glDrawElements(GL_LINES , this->normal_indices.size(), GL_UNSIGNED_INT , &this->normal_indices[0]);
		glBindVertexArray(0);
		glUseProgram(0);
	}

	if (this->GLReturnedError("Scoreboard::Draw - on exit"))
		return;
}

void Scoreboard::DrawScreen(const mat4 & projection, mat4 modelview, const ivec2 & size, GLint level, FrameBufferObject fbo, const float time)
{
		if (this->GLReturnedError("screen::Draw - on entry"))
    return;

  glEnable(GL_DEPTH_TEST);

  modelview = rotate(modelview, time * 120.0f, vec3(0.0f, 1.0f, 0.0f));
  mat4 mvp = projection * modelview;
  mat3 nm = inverse(transpose(mat3(modelview)));

  glBindTexture(GL_TEXTURE_2D, fbo.texture_handles[0]);

  glEnable(GL_TEXTURE_2D);

	glTexEnvf(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_REPLACE);
	glTexParameterf(GL_TEXTURE_2D , GL_TEXTURE_WRAP_S , GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D , GL_TEXTURE_WRAP_T , GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_LINEAR);

  shader.Use();
  shader.CommonSetup(time, value_ptr(size), value_ptr(projection), value_ptr(modelview), value_ptr(mvp), value_ptr(nm));
  glBindVertexArray(this->vertex_array_handle);
  glDrawElements(GL_TRIANGLES , this->vertex_indices.size(), GL_UNSIGNED_INT , &this->vertex_indices[0]);
  glBindVertexArray(0);
  glUseProgram(0);

  if (this->draw_normals)
  {    
    this->solid_color.Use();
    this->solid_color.CommonSetup(time, value_ptr(size), value_ptr(projection), value_ptr(modelview), value_ptr(mvp), value_ptr(nm));
    glBindVertexArray(this->normal_array_handle);
    glDrawElements(GL_LINES , this->normal_indices.size(), GL_UNSIGNED_INT , &this->normal_indices[0]);
    glBindVertexArray(0);
    glUseProgram(0);
  }

  if (this->GLReturnedError("Screen::Draw - on exit"))
    return;

}

