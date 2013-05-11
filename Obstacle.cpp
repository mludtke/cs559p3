/***************************************************************
*	Obstacle
*
*	A box shaped object that can be used as an obstacle
****************************************************************/

#include <iostream>
#include "Obstacle.h"

using namespace std;
using namespace glm;

Obstacle::Obstacle() : Object()
{
	vec4 lighter_color(MakeColor(255, 69, 0, 1.0f));
	vec4 darker_color = vec4(vec3(lighter_color) /* 2.0f / 3.0f*/, 1.0f);
	this->colors[0] = lighter_color;
	this->colors[1] = lighter_color;
}


void Obstacle::BuildNormalVisualizationGeometry()
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

float Obstacle::getTime()
{
	return 0.0f;
}

vec3 Obstacle::getPostion()
{
	return this->position;
}

void Obstacle::setPosition(vec3 pos)
{
	position = pos;
}

float Obstacle::getForce()
{
	return this->force;
}

void Obstacle::setForce(float f)
{
	this->force = f;
}

glm::vec3 Obstacle::getDirection()
{
	return this->direction;
}

void Obstacle::setDirection(vec3 dir)
{
	this->direction = dir;
}

bool Obstacle::Initialize(float height, float width, int shader)
{
	if (this->GLReturnedError("Obstacle::Initialize - on entry"))
		return false;

	if (!super::Initialize())
		return false;

	
  //==================== CubeMap ======================// 
  //          v10 ---- v11 
  //          || top   ||
  // v5 ----- v6 ----- v7 ----- v8 ----- v9
  // || left  || front || right || back  ||
  // v0 ----- v1 ----- v2 ----- v3 ----- v4
  //          || base  ||
  //          v12 ---- v13
  //==================== CubeMap ======================// 
  mat4 MV(1.0f);

  float height_top = height;
  float height_bottom = 0.0f;
  width = width/2.0f;

  vec3 position;
  VertexAttributesPCNT cubeMap[14];
  cubeMap[0].position = vec3(-width,height_bottom,width);
  cubeMap[1].position = vec3(-width,height_bottom,-width);
  cubeMap[2].position = vec3(width,height_bottom,-width);
  cubeMap[3].position = vec3(width,height_bottom,width);
  cubeMap[4].position = vec3(-width,height_bottom,width);
  cubeMap[5].position = vec3(-width,height_top,width);
  cubeMap[6].position = vec3(-width,height_top,-width);
  cubeMap[7].position = vec3(width,height_top,-width);
  cubeMap[8].position = vec3(width,height_top,width);
  cubeMap[9].position = vec3(-width,height_top,width);
  cubeMap[10].position = vec3(-width,height_top,width);
  cubeMap[11].position = vec3(width,height_top,width);
  cubeMap[12].position = vec3(-width,height_bottom,width);
  cubeMap[13].position = vec3(width,height_bottom,width);

  for (int i = 0; i < 14; i++)
    cubeMap[i].position = vec3(MV * vec4(cubeMap[i].position, 1.0f));

  cubeMap[0].texture_coordinate = vec2(0.00f, 0.00f);
  cubeMap[1].texture_coordinate = vec2(1.00f, 0.00f);
  cubeMap[5].texture_coordinate = vec2(0.00f, 1.00f);
  cubeMap[6].texture_coordinate = vec2(1.00f, 1.00f);  
  
  //==================== Left ======================//   
  cubeMap[0].normal = vec3(1.0f,0.0f,0.0f);
  cubeMap[1].normal = vec3(1.0f,0.0f,0.0f);  
  cubeMap[6].normal = vec3(1.0f,0.0f,0.0f);
  cubeMap[5].normal = vec3(1.0f,0.0f,0.0f);  

  this->vertices.push_back(cubeMap[0]);
  this->vertices.push_back(cubeMap[1]);
  this->vertices.push_back(cubeMap[6]);
  this->vertex_indices.push_back(this->vertices.size() - 3);
  this->vertex_indices.push_back(this->vertices.size() - 2);
  this->vertex_indices.push_back(this->vertices.size() - 1);
  this->BuildNormalVisualizationGeometry();
  
  this->vertices.push_back(cubeMap[0]);
  this->vertices.push_back(cubeMap[6]);
  this->vertices.push_back(cubeMap[5]);  
  this->vertex_indices.push_back(this->vertices.size() - 3);
  this->vertex_indices.push_back(this->vertices.size() - 2);
  this->vertex_indices.push_back(this->vertices.size() - 1);
  this->BuildNormalVisualizationGeometry();

  //==================== Right ======================//   
  cubeMap[2].texture_coordinate = vec2(0.00f, 0.00f);
  cubeMap[3].texture_coordinate = vec2(1.00f, 0.00f);
  cubeMap[7].texture_coordinate = vec2(0.00f, 1.00f);
  cubeMap[8].texture_coordinate = vec2(1.00f, 1.00f);  

  cubeMap[2].normal = vec3(0.0f,0.0f,1.0f);
  cubeMap[3].normal = vec3(0.0f,0.0f,1.0f);  
  cubeMap[8].normal = vec3(0.0f,0.0f,1.0f);
  cubeMap[7].normal = vec3(0.0f,0.0f,1.0f);  

  this->vertices.push_back(cubeMap[2]);
  this->vertices.push_back(cubeMap[3]);
  this->vertices.push_back(cubeMap[8]);
  this->vertex_indices.push_back(this->vertices.size() - 3);
  this->vertex_indices.push_back(this->vertices.size() - 2);
  this->vertex_indices.push_back(this->vertices.size() - 1);
  this->BuildNormalVisualizationGeometry();
  
  this->vertices.push_back(cubeMap[2]);
  this->vertices.push_back(cubeMap[8]);
  this->vertices.push_back(cubeMap[7]);  
  this->vertex_indices.push_back(this->vertices.size() - 3);
  this->vertex_indices.push_back(this->vertices.size() - 2);
  this->vertex_indices.push_back(this->vertices.size() - 1);
  this->BuildNormalVisualizationGeometry();

  //==================== Front ======================//   
  cubeMap[1].texture_coordinate = vec2(0.00f, 0.00f);
  cubeMap[2].texture_coordinate = vec2(1.00f, 0.00f);
  cubeMap[6].texture_coordinate = vec2(0.00f, 1.00f);
  cubeMap[7].texture_coordinate = vec2(1.00f, 1.00f);  

  cubeMap[1].normal = vec3(0.0f,0.0f,1.0f);
  cubeMap[2].normal = vec3(0.0f,0.0f,1.0f);  
  cubeMap[7].normal = vec3(0.0f,0.0f,1.0f);
  cubeMap[6].normal = vec3(0.0f,0.0f,1.0f); 

  this->vertices.push_back(cubeMap[1]);
  this->vertices.push_back(cubeMap[2]);
  this->vertices.push_back(cubeMap[7]);
  this->vertex_indices.push_back(this->vertices.size() - 3);
  this->vertex_indices.push_back(this->vertices.size() - 2);
  this->vertex_indices.push_back(this->vertices.size() - 1);
  this->BuildNormalVisualizationGeometry();
  
  this->vertices.push_back(cubeMap[1]);
  this->vertices.push_back(cubeMap[7]);
  this->vertices.push_back(cubeMap[6]);  
  this->vertex_indices.push_back(this->vertices.size() - 3);
  this->vertex_indices.push_back(this->vertices.size() - 2);
  this->vertex_indices.push_back(this->vertices.size() - 1);
  this->BuildNormalVisualizationGeometry();

  //==================== Top ======================//   
  cubeMap[6].texture_coordinate = vec2(0.00f, 0.00f);
  cubeMap[7].texture_coordinate = vec2(1.00f, 0.00f);
  cubeMap[10].texture_coordinate = vec2(0.00f, 1.00f);
  cubeMap[11].texture_coordinate = vec2(1.00f, 1.00f);  

  cubeMap[6].normal = vec3(0.0f,0.0f,1.0f);
  cubeMap[7].normal = vec3(0.0f,0.0f,1.0f);  
  cubeMap[11].normal = vec3(0.0f,0.0f,1.0f);
  cubeMap[10].normal = vec3(0.0f,0.0f,1.0f);  

  this->vertices.push_back(cubeMap[6]);
  this->vertices.push_back(cubeMap[7]);
  this->vertices.push_back(cubeMap[11]);
  this->vertex_indices.push_back(this->vertices.size() - 3);
  this->vertex_indices.push_back(this->vertices.size() - 2);
  this->vertex_indices.push_back(this->vertices.size() - 1);
  this->BuildNormalVisualizationGeometry();
  
  this->vertices.push_back(cubeMap[6]);
  this->vertices.push_back(cubeMap[11]);
  this->vertices.push_back(cubeMap[10]);  
  this->vertex_indices.push_back(this->vertices.size() - 3);
  this->vertex_indices.push_back(this->vertices.size() - 2);
  this->vertex_indices.push_back(this->vertices.size() - 1);
  this->BuildNormalVisualizationGeometry();

  //==================== Base ======================//   
  cubeMap[0].texture_coordinate = vec2(0.00f, 0.00f);
  cubeMap[1].texture_coordinate = vec2(1.00f, 0.00f);
  cubeMap[5].texture_coordinate = vec2(0.00f, 1.00f);
  cubeMap[6].texture_coordinate = vec2(1.00f, 1.00f);  

  cubeMap[12].normal = vec3(0.0f,0.0f,1.0f);
  cubeMap[13].normal = vec3(0.0f,0.0f,1.0f);  
  cubeMap[2].normal = vec3(0.0f,0.0f,1.0f);
  cubeMap[1].normal = vec3(0.0f,0.0f,1.0f);  

  this->vertices.push_back(cubeMap[12]);
  this->vertices.push_back(cubeMap[13]);
  this->vertices.push_back(cubeMap[2]);
  this->vertex_indices.push_back(this->vertices.size() - 3);
  this->vertex_indices.push_back(this->vertices.size() - 2);
  this->vertex_indices.push_back(this->vertices.size() - 1);
  this->BuildNormalVisualizationGeometry();
  
  this->vertices.push_back(cubeMap[12]);
  this->vertices.push_back(cubeMap[2]);
  this->vertices.push_back(cubeMap[1]);  
  this->vertex_indices.push_back(this->vertices.size() - 3);
  this->vertex_indices.push_back(this->vertices.size() - 2);
  this->vertex_indices.push_back(this->vertices.size() - 1);
  this->BuildNormalVisualizationGeometry();

  //==================== Back ======================//   
  cubeMap[3].texture_coordinate = vec2(0.00f, 0.00f);
  cubeMap[4].texture_coordinate = vec2(1.00f, 0.00f);
  cubeMap[8].texture_coordinate = vec2(0.00f, 1.00f);
  cubeMap[9].texture_coordinate = vec2(1.00f, 1.00f);  

  cubeMap[3].normal = vec3(0.0f,0.0f,1.0f);
  cubeMap[4].normal = vec3(0.0f,0.0f,1.0f);  
  cubeMap[9].normal = vec3(0.0f,0.0f,1.0f);
  cubeMap[8].normal = vec3(0.0f,0.0f,1.0f);  

  this->vertices.push_back(cubeMap[3]);
  this->vertices.push_back(cubeMap[4]);
  this->vertices.push_back(cubeMap[9]);
  this->vertex_indices.push_back(this->vertices.size() - 3);
  this->vertex_indices.push_back(this->vertices.size() - 2);
  this->vertex_indices.push_back(this->vertices.size() - 1);
  this->BuildNormalVisualizationGeometry();
  
  this->vertices.push_back(cubeMap[3]);
  this->vertices.push_back(cubeMap[9]);
  this->vertices.push_back(cubeMap[8]);  
  this->vertex_indices.push_back(this->vertices.size() - 3);
  this->vertex_indices.push_back(this->vertices.size() - 2);
  this->vertex_indices.push_back(this->vertices.size() - 1);
  this->BuildNormalVisualizationGeometry();

  if (!this->PostGLInitialize(&this->vertex_array_handle, &this->vertex_coordinate_handle, this->vertices.size() * sizeof(VertexAttributesPCNT), &this->vertices[0]))
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

  if (!this->shader.Initialize("./shaders/basic_texture_shader.vert", "./shaders/basic_texture_shader.frag"))
    return false;

  if (!this->solid_color.Initialize("./shaders/solid_shader.vert", "./shaders/solid_shader.frag"))
    return false;

	if (this->GLReturnedError("Obstacle::Initialize - on exit"))
		return false;


	return true;
}

void Obstacle::TakeDown()
{
	this->vertices.clear();
	this->shader.TakeDown();
	this->adsShader.TakeDown();
	this->solid_color.TakeDown();
	super::TakeDown();
}

void Obstacle::Draw(const ivec2 & size)
{
	assert(false);
}

void Obstacle::Draw(const mat4 & projection, mat4 modelview, const ivec2 & size, GLint level, GLint shade, const float time)
{
	if (this->GLReturnedError("Obstacle::Draw - on entry"))
    return;

  glEnable(GL_DEPTH_TEST);

  //modelview = rotate(modelview, time * 30.0f, vec3(1.0f, 0.0f, 0.0f));
  modelview = rotate(modelview, time * 120.0f, vec3(0.0f, 1.0f, 0.0f));
  mat4 mvp = projection * modelview;
  mat3 nm = inverse(transpose(mat3(modelview)));

  TextureManager::Inst()->BindTexture(0,0);	//Bind it to the crate texture

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

  if (this->GLReturnedError("Obstacle::Draw - on exit"))
    return;
}


