/*	

*/

#include <iostream>
#include "map.h"

using namespace std;
using namespace glm;

Map::Map() : Object()
{
	vec4 lighter_color(MakeColor(255, 69, 0, 1.0f));
	vec4 darker_color = vec4(vec3(lighter_color) /* 2.0f / 3.0f*/, 1.0f);
	this->colors[0] = lighter_color;
	this->colors[1] = lighter_color;
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

bool Map::InitializeFloor(bool minimap)
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
		color = GREEN;
	else
	    color = GREEN;
	vec3 normal_floor = vec3(0.0f, 1.0f, 0.0f);

	x = -105.0f;
	y = 0.0f;
	z = 105.0f;

	int steps = 15;
	float increment = height / (float)steps;

	VertexAttributesPCNT bottom_left_vertex, top_left_vertex, bottom_right_vertex, top_right_vertex;

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

	
	if (!minimap) //special effects should not be done on minimap
	{
		if (!this->shader.Initialize("phong_shader.vert", "phong_shader.frag"))
			return false;
		/*if(!this->shader.Initialize("rainbow.vert", "rainbow.frag"))
			return false;*/
		/*if(!this->shader.Initialize("water_shader.vert", "water_shader.frag"))
			return false;*/
	}
	else //need basic shading for the minimap
	{
		if (!this->shader.Initialize("solid_shader.vert", "solid_shader.frag"))
			return false;
	}

	if (this->GLReturnedError("Floor::Initialize - on exit"))
		return false;
	return true;
}

bool Map::InitializeWalls()
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

	if (!this->shader.Initialize("basic_texture_shader.vert", "basic_texture_shader.frag"))
		return false;

	if (!this->solid_color.Initialize("solid_shader.vert", "solid_shader.frag"))
		return false;

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

	shader.Use();
	shader.CommonSetup(time, value_ptr(size), value_ptr(projection), value_ptr(modelview), value_ptr(mvp), value_ptr(nm));

/*	if(shade == 2)
	{
		adsShader.Use();
		adsShader.CommonSetup(time, value_ptr(size), value_ptr(projection), value_ptr(modelview), value_ptr(mvp), value_ptr(nm));

		//adsShader.SetLight(glm::vec4(0.0f, 0.0f, 2.0f, 1.0f), glm::vec3(0.2f), glm::vec3(0.7f), vec3(0.7f));
		//adsShader.SetMaterial(vec3(0.0f, 0.0f, 0.0f), vec3(0.588235f, 0.670588f, 0.729412f), vec3(0.9f, 0.9f, 0.9f), 96.0f);
		adsShader.SetLight(glm::vec4(0.0f, 0.0f, 2.0f, 1.0f), glm::vec3(0.2f), glm::vec3(0.7f), vec3(0.7f));
		adsShader.SetMaterial(vec3(0.24725f, 0.21995f, 0.0745f), vec3(0.75164f, 0.60648f, 0.22648f), vec3(0.628281f, 0.555802f, 0.366065f), 51.2f);
	}
	if(shade == 3)
	{
		adsShader.Use();
		adsShader.CommonSetup(time, value_ptr(size), value_ptr(projection), value_ptr(modelview), value_ptr(mvp), value_ptr(nm));
		adsShader.SetLight(glm::vec4(0.0f, 0.0f, 2.0f, 1.0f), glm::vec3(0.2f), glm::vec3(0.7f), vec3(0.7f));
		adsShader.SetMaterial(vec3(0.24725f, 0.21995f, 0.0745f), vec3(0.75164f, 0.60648f, 0.22648f), vec3(0.628281f, 0.555802f, 0.366065f), 51.2f);
	}*/
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


