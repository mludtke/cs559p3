/*************************************************************	
*	map
*
*	Map objects. Specifically floors and walls. 
*
**************************************************************/

#include <iostream>
#include "map.h"

using namespace std;
using namespace glm;

Map::Map() : Object()
{
	this->shader_index = 0;
}

void Map::StepShader()
{
	this->shader_index = ++this->shader_index % this->shaders.size();	
}

void Map::BuildNormalVisualizationGeometry()
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

bool Map::InitializeFloor(bool minimap, bool clock, bool floor)
{
	if (this->GLReturnedError("Floor::Initialize - on entry"))
		return false;

	if (!super::Initialize())
		return false;

	mat4 m;
	const vec3 n = normalize(vec3(1.0f, 1.0f, 1.0f));

	const float height = 210.0f;	//mile in modelview coordinates
	float theta = 0.0f, phi = 0.0f;
	float x, y, z;
	vec4 location;
	vec3 color;
	if (!minimap)
		color = BLUE;
	else
	    color = GREEN;
	vec3 normal_floor = vec3(0.0f, 1.0f, 0.0f);

	x = -105.0f;
	y = 0.0f;
	z = 105.0f;

	int steps = 1;
	float increment = height / (float)steps;

	VertexAttributesPCNT bottom_left_vertex, top_left_vertex, bottom_right_vertex, top_right_vertex;

	bottom_left_vertex.texture_coordinate = vec2(0.0f, 0.0f);
	bottom_right_vertex.texture_coordinate = vec2(1.0f, 0.0f);
	top_left_vertex.texture_coordinate = vec2(0.0f, 1.0f);
	top_right_vertex.texture_coordinate = vec2(1.0f, 1.0f);


	//create the floor coordinates
	for (int i = 0; i < steps; i++)
	{
		for (int j = 0; j < steps; j++)
		{
		
			location = vec4(x, y, z, 1.0f);
			bottom_left_vertex.position = vec3(m * location);
			bottom_left_vertex.color = color;
			bottom_left_vertex.normal = vec3(normal_floor);

			x = x + increment;
			location = vec4(x, y, z, 1.0f);
			bottom_right_vertex.position = vec3(m * location);
			bottom_right_vertex.color = color;
			bottom_right_vertex.normal = vec3(normal_floor);

			z = z - increment;
			location = vec4(x, y, z, 1.0f);
			top_right_vertex.position = vec3(m * location);
			top_right_vertex.color = color;
			top_right_vertex.normal = vec3(normal_floor);

			x = x - increment;
			location = vec4(x, y, z, 1.0f);
			top_left_vertex.position = vec3(m * location);
			top_left_vertex.color = color;
			top_left_vertex.normal = vec3(normal_floor);

			//Draw the floor
			this->vertices.push_back(bottom_left_vertex);
			this->vertices.push_back(top_left_vertex);
			this->vertices.push_back(bottom_right_vertex);
			this->vertices.push_back(top_right_vertex);

			this->vertex_indices.push_back(vertices.size() - 1);
			this->vertex_indices.push_back(vertices.size() - 3);
			this->vertex_indices.push_back(vertices.size() - 4);
			
			this->vertex_indices.push_back(vertices.size() - 1);
			this->vertex_indices.push_back(vertices.size() - 4);
			this->vertex_indices.push_back(vertices.size() - 2);

			BuildNormalVisualizationGeometry();
			x = x + increment;
			z = z + increment;
		}
		x = -105.0f;
		z = z - increment;
	}



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

	
	if (minimap) //special effects should not be done on minimap
	{
		if (!this->shader.Initialize("./shaders/solid_shader.vert", "./shaders/solid_shader.frag"))
			return false;
		
		this->shaders.push_back(&shader);
	}
	else if (clock)
	{
		if(!this->shader.Initialize("./shaders/clock.vert", "./shaders/clock.frag"))
			return false;
		this->shaders.push_back(&shader);
	}
	else if (floor)
	{
		if(!this->shader.Initialize("./shaders/basic_texture_shader.vert", "./shaders/basic_texture_shader.frag"))
			return false;

		this->shaders.push_back(&shader);
	}
	else //floor shader
	{
		assert(this->rainbow.Initialize("./shaders/rainbow.vert", "./shaders/rainbow.frag"));
		assert(this->color_changer.Initialize("./shaders/color_changer_shader.vert", "./shaders/color_changer_shader.frag"));
		assert(this->space.Initialize("./shaders/space.vert", "./shaders/space.frag"));
	
		this->shaders.push_back(&space);
		this->shaders.push_back(&color_changer);
		this->shaders.push_back(&rainbow);
	}

	if (this->GLReturnedError("Floor::Initialize - on exit"))
		return false;
	return true;
}

bool Map::InitializeWalls(bool minimap)
{
	if (this->GLReturnedError("Walls::Initialize - on entry"))
		return false;

	if (!super::Initialize())
		return false;

	mat4 m;
	const vec3 n = normalize(vec3(1.0f, 1.0f, 1.0f));

	const float radius = 1.0f;
	float theta = 0.0f, phi = 0.0f;
	float x, y, z;
	vec4 location;
	vec3 color = RED;
	vec3 color2 = BLUE;
	vec3 normal_floor = vec3(0.0f, 1.0f, 0.0f);

	//create the wall (wall one)
	VertexAttributesPCNT bottom_left_vertex, top_left_vertex, bottom_right_vertex, top_right_vertex,
		back_top_left, back_top_right, back_bottom_left, back_bottom_right;

	top_left_vertex.texture_coordinate = vec2(0.00f, 0.75f);
	top_right_vertex.texture_coordinate = vec2(1.00f, 0.75f);
	bottom_left_vertex.texture_coordinate = vec2(0.00f, 0.25f);
	bottom_right_vertex.texture_coordinate = vec2(1.00f, 0.25f);

	back_bottom_left.texture_coordinate = vec2(0.00f, 0.00f);
	back_bottom_right.texture_coordinate = vec2(1.00f, 0.00f);
	back_top_left.texture_coordinate = vec2(0.00f, 1.00f);
	back_top_right.texture_coordinate = vec2(1.00f, 1.00f);

	x = -105.5f;
	y = 2.0f;
	z = -105.0f;
	
	location = vec4(x, y, z, 1.0f);
	top_left_vertex.position = vec3(m * location);
	top_left_vertex.color = color2;
	top_left_vertex.normal = vec3(0.0f, 0.0f, 1.0f);

	x = 105.5f;
	location = vec4(x, y, z, 1.0f);
	top_right_vertex.position = vec3(m * location);
	top_right_vertex.color = color2;
	top_right_vertex.normal = vec3(0.0f, 0.0f, 1.0f);

	y = 0.0f;
	location = vec4(x, y, z, 1.0f);
	bottom_right_vertex.position = vec3(m * location);
	bottom_right_vertex.color = color2;
	bottom_right_vertex.normal = vec3(0.0f, 0.0f, 1.0f);

	x = -105.5f;
	location = vec4(x, y, z, 1.0f);
	bottom_left_vertex.position = vec3(m * location);
	bottom_left_vertex.color = color2;
	bottom_left_vertex.normal = vec3(0.0f, 0.0f, 1.0f);

	z = -105.5f;
	location = vec4(x, y, z, 1.0f);
	back_bottom_left.position = vec3(m * location);
	back_bottom_left.color = color2;
	back_bottom_left.normal = vec3(0.0f, 0.0f, -1.0f);

	x = 105.5f;
	location = vec4(x, y, z, 1.0f);
	back_bottom_right.position = vec3(m * location);
	back_bottom_right.color = color2;
	back_bottom_right.normal = vec3(0.0f, 0.0f, -1.0f);

	y = 2.0f;
	location = vec4(x, y, z, 1.0f);
	back_top_right.position = vec3(m * location);
	back_top_right.color = color2;
	back_top_right.normal = vec3(0.0f, 0.0f, -1.0f);

	x = -105.5f;
	location = vec4(x, y, z, 1.0f);
	back_top_left.position = vec3(m * location);
	back_top_left.color = color2;
	back_top_left.normal = vec3(0.0f, 0.0f, -1.0f);

	this->vertices.push_back(back_top_left);
	this->vertices.push_back(back_top_right);
	this->vertices.push_back(back_bottom_left);
	this->vertices.push_back(back_bottom_right);

	this->vertices.push_back(top_left_vertex);
	this->vertices.push_back(bottom_left_vertex);
	this->vertices.push_back(top_right_vertex);
	this->vertices.push_back(bottom_right_vertex);

	//arena facing side
	this->vertex_indices.push_back(vertices.size() - 1);
	this->vertex_indices.push_back(vertices.size() - 4);
	this->vertex_indices.push_back(vertices.size() - 3);
			
	this->vertex_indices.push_back(vertices.size() - 1);
	this->vertex_indices.push_back(vertices.size() - 2);
	this->vertex_indices.push_back(vertices.size() - 4);
	//top
	this->vertex_indices.push_back(vertices.size() - 2);
	this->vertex_indices.push_back(vertices.size() - 4);
	this->vertex_indices.push_back(vertices.size() - 8);
			
	this->vertex_indices.push_back(vertices.size() - 7);
	this->vertex_indices.push_back(vertices.size() - 8);
	this->vertex_indices.push_back(vertices.size() - 2);
	//back side
	this->vertex_indices.push_back(vertices.size() - 8);
	this->vertex_indices.push_back(vertices.size() - 6);
	this->vertex_indices.push_back(vertices.size() - 5);
			
	this->vertex_indices.push_back(vertices.size() - 7);
	this->vertex_indices.push_back(vertices.size() - 8);
	this->vertex_indices.push_back(vertices.size() - 5);


	//wall 2
	x = -105.0f;
	y = 2.0f;
	z = -105.5f;
	
	location = vec4(x, y, z, 1.0f);
	top_right_vertex.position = vec3(m * location);
	top_right_vertex.color = color2;
	top_right_vertex.normal = vec3(1.0f, 0.0f, 0.0f);

	z = 105.5f;
	location = vec4(x, y, z, 1.0f);
	top_left_vertex.position = vec3(m * location);
	top_left_vertex.color = color2;
	top_left_vertex.normal = vec3(1.0f, 0.0f, 0.0f);

	y = 0.0f;
	location = vec4(x, y, z, 1.0f);
	bottom_left_vertex.position = vec3(m * location);
	bottom_left_vertex.color = color2;
	bottom_left_vertex.normal = vec3(1.0f, 0.0f, 0.0f);

	z = -105.5f;
	location = vec4(x, y, z, 1.0f);
	bottom_right_vertex.position = vec3(m * location);
	bottom_right_vertex.color = color2;
	bottom_right_vertex.normal = vec3(1.0f, 0.0f, 0.0f);

	x = -105.5f;
	location = vec4(x, y, z, 1.0f);
	back_bottom_right.position = vec3(m * location);
	back_bottom_right.color = color2;
	back_bottom_right.normal = vec3(0.0f, 0.0f, -1.0f);

	z = 105.5f;
	location = vec4(x, y, z, 1.0f);
	back_bottom_left.position = vec3(m * location);
	back_bottom_left.color = color2;
	back_bottom_left.normal = vec3(0.0f, 0.0f, -1.0f);

	y = 2.0f;
	location = vec4(x, y, z, 1.0f);
	back_top_right.position = vec3(m * location);
	back_top_right.color = color2;
	back_top_right.normal = vec3(0.0f, 0.0f, -1.0f);

	z = -105.5f;
	location = vec4(x, y, z, 1.0f);
	back_top_left.position = vec3(m * location);
	back_top_left.color = color2;
	back_top_left.normal = vec3(0.0f, 0.0f, -1.0f);

	this->vertices.push_back(back_top_left);
	this->vertices.push_back(back_top_right);
	this->vertices.push_back(back_bottom_left);
	this->vertices.push_back(back_bottom_right);

	this->vertices.push_back(top_left_vertex);
	this->vertices.push_back(bottom_left_vertex);
	this->vertices.push_back(top_right_vertex);
	this->vertices.push_back(bottom_right_vertex);

	//arena facing side
	this->vertex_indices.push_back(vertices.size() - 1);
	this->vertex_indices.push_back(vertices.size() - 4);
	this->vertex_indices.push_back(vertices.size() - 3);
			
	this->vertex_indices.push_back(vertices.size() - 1);
	this->vertex_indices.push_back(vertices.size() - 2);
	this->vertex_indices.push_back(vertices.size() - 4);
	//top
	this->vertex_indices.push_back(vertices.size() - 2);
	this->vertex_indices.push_back(vertices.size() - 4);
	this->vertex_indices.push_back(vertices.size() - 8);
			
	this->vertex_indices.push_back(vertices.size() - 7);
	this->vertex_indices.push_back(vertices.size() - 8);
	this->vertex_indices.push_back(vertices.size() - 2);
	//back side
	this->vertex_indices.push_back(vertices.size() - 8);
	this->vertex_indices.push_back(vertices.size() - 6);
	this->vertex_indices.push_back(vertices.size() - 5);
			
	this->vertex_indices.push_back(vertices.size() - 7);
	this->vertex_indices.push_back(vertices.size() - 8);
	this->vertex_indices.push_back(vertices.size() - 5);

	//wall 3
	x = -105.5f;
	y = 2.0f;
	z = 105.0f;
	
	location = vec4(x, y, z, 1.0f);
	top_right_vertex.position = vec3(m * location);
	top_right_vertex.color = color2;
	top_right_vertex.normal = vec3(0.0f, 0.0f, -1.0f);

	x = 105.5f;
	location = vec4(x, y, z, 1.0f);
	top_left_vertex.position = vec3(m * location);
	top_left_vertex.color = color2;
	top_left_vertex.normal = vec3(0.0f, 0.0f, -1.0f);

	y = 0.0f;
	location = vec4(x, y, z, 1.0f);
	bottom_left_vertex.position = vec3(m * location);
	bottom_left_vertex.color = color2;
	bottom_left_vertex.normal = vec3(0.0f, 0.0f, -1.0f);

	x = -105.5f;
	location = vec4(x, y, z, 1.0f);
	bottom_right_vertex.position = vec3(m * location);
	bottom_right_vertex.color = color2;
	bottom_right_vertex.normal = vec3(0.0f, 0.0f, -1.0f);

	z = 105.5f;
	location = vec4(x, y, z, 1.0f);
	back_bottom_right.position = vec3(m * location);
	back_bottom_right.color = color2;
	back_bottom_right.normal = vec3(0.0f, 0.0f, -1.0f);

	x = 105.5f;
	location = vec4(x, y, z, 1.0f);
	back_bottom_left.position = vec3(m * location);
	back_bottom_left.color = color2;
	back_bottom_left.normal = vec3(0.0f, 0.0f, -1.0f);

	y = 2.0f;
	location = vec4(x, y, z, 1.0f);
	back_top_left.position = vec3(m * location);
	back_top_left.color = color2;
	back_top_left.normal = vec3(0.0f, 0.0f, -1.0f);

	x = -105.5f;
	location = vec4(x, y, z, 1.0f);
	back_top_right.position = vec3(m * location);
	back_top_right.color = color2;
	back_top_right.normal = vec3(0.0f, 0.0f, -1.0f);

	this->vertices.push_back(back_top_left);
	this->vertices.push_back(back_top_right);
	this->vertices.push_back(back_bottom_left);
	this->vertices.push_back(back_bottom_right);

	this->vertices.push_back(top_left_vertex);
	this->vertices.push_back(bottom_left_vertex);
	this->vertices.push_back(top_right_vertex);
	this->vertices.push_back(bottom_right_vertex);

	//arena facing side
	this->vertex_indices.push_back(vertices.size() - 1);
	this->vertex_indices.push_back(vertices.size() - 4);
	this->vertex_indices.push_back(vertices.size() - 3);
			
	this->vertex_indices.push_back(vertices.size() - 1);
	this->vertex_indices.push_back(vertices.size() - 2);
	this->vertex_indices.push_back(vertices.size() - 4);
	//top
	this->vertex_indices.push_back(vertices.size() - 2);
	this->vertex_indices.push_back(vertices.size() - 4);
	this->vertex_indices.push_back(vertices.size() - 8);
			
	this->vertex_indices.push_back(vertices.size() - 7);
	this->vertex_indices.push_back(vertices.size() - 8);
	this->vertex_indices.push_back(vertices.size() - 2);
	//back side
	this->vertex_indices.push_back(vertices.size() - 8);
	this->vertex_indices.push_back(vertices.size() - 6);
	this->vertex_indices.push_back(vertices.size() - 5);
			
	this->vertex_indices.push_back(vertices.size() - 7);
	this->vertex_indices.push_back(vertices.size() - 8);
	this->vertex_indices.push_back(vertices.size() - 5);


	//wall 4
	x = 105.0f;
	y = 2.0f;
	z = -105.5f;
	
	location = vec4(x, y, z, 1.0f);
	top_left_vertex.position = vec3(m * location);
	top_left_vertex.color = color2;
	top_left_vertex.normal = vec3(-1.0f, 0.0f, 0.0f);

	z = 105.5f;
	location = vec4(x, y, z, 1.0f);
	top_right_vertex.position = vec3(m * location);
	top_right_vertex.color = color2;
	top_right_vertex.normal = vec3(-1.0f, 0.0f, 0.0f);

	y = 0.0f;
	location = vec4(x, y, z, 1.0f);
	bottom_right_vertex.position = vec3(m * location);
	bottom_right_vertex.color = color2;
	bottom_right_vertex.normal = vec3(-1.0f, 0.0f, 0.0f);

	z = -105.5f;
	location = vec4(x, y, z, 1.0f);
	bottom_left_vertex.position = vec3(m * location);
	bottom_left_vertex.color = color2;
	bottom_left_vertex.normal = vec3(-1.0f, 0.0f, 0.0f);

	x = 105.5f;
	location = vec4(x, y, z, 1.0f);
	back_bottom_left.position = vec3(m * location);
	back_bottom_left.color = color2;
	back_bottom_left.normal = vec3(0.0f, 0.0f, -1.0f);

	z = 105.5f;
	location = vec4(x, y, z, 1.0f);
	back_bottom_right.position = vec3(m * location);
	back_bottom_right.color = color2;
	back_bottom_right.normal = vec3(0.0f, 0.0f, -1.0f);

	y = 2.0f;
	location = vec4(x, y, z, 1.0f);
	back_top_right.position = vec3(m * location);
	back_top_right.color = color2;
	back_top_right.normal = vec3(0.0f, 0.0f, -1.0f);

	z = -105.5f;
	location = vec4(x, y, z, 1.0f);
	back_top_left.position = vec3(m * location);
	back_top_left.color = color2;
	back_top_left.normal = vec3(0.0f, 0.0f, -1.0f);

	this->vertices.push_back(back_top_left);
	this->vertices.push_back(back_top_right);
	this->vertices.push_back(back_bottom_left);
	this->vertices.push_back(back_bottom_right);

	this->vertices.push_back(top_left_vertex);
	this->vertices.push_back(bottom_left_vertex);
	this->vertices.push_back(top_right_vertex);
	this->vertices.push_back(bottom_right_vertex);

	//arena facing side
	this->vertex_indices.push_back(vertices.size() - 1);
	this->vertex_indices.push_back(vertices.size() - 4);
	this->vertex_indices.push_back(vertices.size() - 3);
			
	this->vertex_indices.push_back(vertices.size() - 1);
	this->vertex_indices.push_back(vertices.size() - 2);
	this->vertex_indices.push_back(vertices.size() - 4);
	//top
	this->vertex_indices.push_back(vertices.size() - 2);
	this->vertex_indices.push_back(vertices.size() - 4);
	this->vertex_indices.push_back(vertices.size() - 8);
			
	this->vertex_indices.push_back(vertices.size() - 7);
	this->vertex_indices.push_back(vertices.size() - 8);
	this->vertex_indices.push_back(vertices.size() - 2);
	//back side
	this->vertex_indices.push_back(vertices.size() - 8);
	this->vertex_indices.push_back(vertices.size() - 6);
	this->vertex_indices.push_back(vertices.size() - 5);
			
	this->vertex_indices.push_back(vertices.size() - 7);
	this->vertex_indices.push_back(vertices.size() - 8);
	this->vertex_indices.push_back(vertices.size() - 5);


	
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

	if (minimap)
	{
		if (!this->shader.Initialize("./shaders/solid_shader_white.vert", "./shaders/solid_shader_white.frag"))
		return false;
	}
	if (this->GLReturnedError("Walls::Initialize - on exit"))
		return false;
	return true;
}



void Map::TakeDown()
{
	this->vertices.clear();
	this->shader.TakeDown();
	this->adsShader.TakeDown();
	this->solid_color.TakeDown();
	super::TakeDown();
}

void Map::Draw(const ivec2 & size)
{
	assert(false);
}

void Map::Draw(const mat4 & projection, mat4 modelview, const ivec2 & size, GLint level, GLint shade, const float time)
{
	if (this->GLReturnedError("Floor::Draw - on entry"))
		return;

	glEnable(GL_DEPTH_TEST);

	mat4 mvp = projection * modelview;
	mat3 nm = inverse(transpose(mat3(modelview)));

	this->shaders[this->shader_index]->Use();
	this->shaders[this->shader_index]->CommonSetup(time, value_ptr(size), value_ptr(projection), value_ptr(modelview), value_ptr(mvp), value_ptr(nm));  

	//cout << "window size: " << size << endl;

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

	if (this->GLReturnedError("Floor::Draw - on exit"))
		return;
}

void Map::Draw_floors(const mat4 & projection, mat4 modelview, const ivec2 & size, FrameBufferObject fbo, float time)
{
	if (this->GLReturnedError("floor::Draw - on entry"))
    return;

  glEnable(GL_DEPTH_TEST);

  //modelview = rotate(modelview, time * 120.0f, vec3(0.0f, 1.0f, 0.0f));
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

  if (this->GLReturnedError("floor::Draw - on exit"))
    return;
}

void Map::Draw_walls(const mat4 & projection, mat4 modelview, const ivec2 & size, const float time)
{
	if (this->GLReturnedError("walls::Draw - on entry"))
    return;

  glEnable(GL_DEPTH_TEST);

  modelview = rotate(modelview, time * 120.0f, vec3(0.0f, 1.0f, 0.0f));
  mat4 mvp = projection * modelview;
  mat3 nm = inverse(transpose(mat3(modelview)));

  TextureManager::Inst()->BindTexture(8,0);	//Bind it to the crate texture

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

  if (this->GLReturnedError("walls::Draw - on exit"))
    return;
}


