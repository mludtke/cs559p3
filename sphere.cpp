#include <iostream>
#include "Sphere.h"

using namespace std;
using namespace glm;

Sphere::Sphere() : Object()
{
	this->shader_index = 0;
}

void Sphere::StepShader()
{
	this->shader_index = ++this->shader_index % this->shaders.size();
}

void Sphere::BuildNormalVisualizationGeometry()
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

void Sphere::hit(float time)
{
	if(!is_hit)
	{
		this->is_hit = true;
	}
	this->time = time;
}

bool Sphere::is_sphere_hit()
{
	return this->is_hit;
}

float Sphere::getTime()
{
	return this->time;
}

void Sphere::setTime(float t)
{
	this->time = t;
	if (t <= 0.0f)
	{
		this->is_hit = false;
	}
}

vec3 Sphere::getPostion()
{
	return this->position;
}

void Sphere::setPosition(vec3 pos)
{
	position = pos;
}

bool Sphere::Initialize(int slices, int stacks, float radius, int shader, int hit, float born_time)
{
	if (this->GLReturnedError("Sphere::Initialize - on entry"))
		return false;

	if (!super::Initialize())
		return false;

	if (hit == 0)
		this->is_hit = false;
	else
		this->is_hit = true;

	if (slices < 0)
		slices = 1;

	this->time = born_time;
	this->position = position;

	//slices *= 4;
	mat4 m;
	const vec3 n = normalize(vec3(1.0f, 1.0f, 1.0f));
	const int stacks_sphere = stacks;
	float theta = 0.0f, phi = 0.0f;
	float x, y, z;
	vec4 location;
	vec3 color = GREEN;
	if (is_hit)
		color = RED;
	if (hit == 2)
		color = BLUE;
	if (hit == 3)
		color = GRAY;

	const float increment_stacks = (3.14159f) / float(stacks_sphere);
	const float increment_slices = (3.14159f) / slices;

	const int mesh_size = slices * stacks;
	std::vector<VertexAttributesPCN> mesh;

	for (int i = 0; i < mesh_size; ++i)
	{
		VertexAttributesPCN mesh_temp;
		mesh.push_back(mesh_temp);
	}

	//top half
	for(int i = 0; i < slices; ++i)
	{

		for(int j = 0; j < stacks_sphere; ++j)
		{
			VertexAttributesPCN cur_bottom_vertex, cur_top_vertex, nxt_bottom_vertex, nxt_top_vertex;

			x = radius * cos(theta) * sin(phi);
			y = radius * sin(theta) * sin(phi);
			z = radius * cos(phi);
			location = vec4(x, y, z, 1.0f);

			cur_bottom_vertex.position = vec3(m * location);
			cur_bottom_vertex.color = color;
			cur_bottom_vertex.normal = vec3(normalize(m * location));

			phi += increment_slices;

			x = radius * cos(theta) * sin(phi);
			y = radius * sin(theta) * sin(phi);
			z = radius * cos(phi);
			location = vec4(x, y, z, 1.0f);

			nxt_bottom_vertex.position = vec3(m * location);
			nxt_bottom_vertex.color = color;
			nxt_bottom_vertex.normal = vec3(normalize(m * location));

			theta += increment_stacks;

			x = radius * cos(theta) * sin(phi);
			y = radius * sin(theta) * sin(phi);
			z = radius * cos(phi);
			location = vec4(x, y, z, 1.0f);

			nxt_top_vertex.position = vec3(m * location);
			nxt_top_vertex.color = color;
			nxt_top_vertex.normal = vec3(normalize(m * location));

			phi -= increment_slices;

			x = radius * cos(theta) * sin(phi);
			y = radius * sin(theta) * sin(phi);
			z = radius * cos(phi);
			location = vec4(x, y, z, 1.0f);

			cur_top_vertex.position = vec3(m * location);
			cur_top_vertex.color = color;
			cur_top_vertex.normal = vec3(normalize(m * location));


			this->vertices.push_back(cur_bottom_vertex);
			this->vertices.push_back(cur_top_vertex);
			this->vertices.push_back(nxt_bottom_vertex);
			this->vertices.push_back(nxt_top_vertex);

			this->vertex_indices.push_back(vertices.size() - 1);
			this->vertex_indices.push_back(vertices.size() - 3);
			this->vertex_indices.push_back(vertices.size() - 4);

			this->vertex_indices.push_back(vertices.size() - 1);
			this->vertex_indices.push_back(vertices.size() - 4);
			this->vertex_indices.push_back(vertices.size() - 2);

			mesh.at(i) = cur_top_vertex;
			mesh.at(i + stacks) = cur_bottom_vertex;

			BuildNormalVisualizationGeometry();

		}
		theta = 0.0f;
		phi += increment_slices;

	}
	m = rotate(m, 180.0f, vec3(0.0f, 1.0f, 0.0f));

	//bottom half
	for(int i = 0; i < slices; ++i)
	{

		for(int j = 0; j < stacks_sphere; ++j)
		{
			VertexAttributesPCN cur_bottom_vertex, cur_top_vertex, nxt_bottom_vertex, nxt_top_vertex;

			x = radius * cos(theta) * sin(phi);
			y = radius * sin(theta) * sin(phi);
			z = radius * cos(phi);
			location = vec4(x, y, z, 1.0f);

			cur_bottom_vertex.position = vec3(m * location);
			cur_bottom_vertex.color = color;
			cur_bottom_vertex.normal = vec3(normalize(m * location));

			phi += increment_slices;

			x = radius * cos(theta) * sin(phi);
			y = radius * sin(theta) * sin(phi);
			z = radius * cos(phi);
			location = vec4(x, y, z, 1.0f);

			nxt_bottom_vertex.position = vec3(m * location);
			nxt_bottom_vertex.color = color;
			nxt_bottom_vertex.normal = vec3(normalize(m * location));

			theta += increment_stacks;

			x = radius * cos(theta) * sin(phi);
			y = radius * sin(theta) * sin(phi);
			z = radius * cos(phi);
			location = vec4(x, y, z, 1.0f);

			nxt_top_vertex.position = vec3(m * location);
			nxt_top_vertex.color = color;
			nxt_top_vertex.normal = vec3(normalize(m * location));

			phi -= increment_slices;

			x = radius * cos(theta) * sin(phi);
			y = radius * sin(theta) * sin(phi);
			z = radius * cos(phi);
			location = vec4(x, y, z, 1.0f);

			cur_top_vertex.position = vec3(m * location);
			cur_top_vertex.color = color;
			cur_top_vertex.normal = vec3(normalize(m * location));


			this->vertices.push_back(cur_bottom_vertex);
			this->vertices.push_back(cur_top_vertex);
			this->vertices.push_back(nxt_bottom_vertex);
			this->vertices.push_back(nxt_top_vertex);

			this->vertex_indices.push_back(vertices.size() - 1);
			this->vertex_indices.push_back(vertices.size() - 2);
			this->vertex_indices.push_back(vertices.size() - 3);

			this->vertex_indices.push_back(vertices.size() - 3);
			this->vertex_indices.push_back(vertices.size() - 2);
			this->vertex_indices.push_back(vertices.size() - 4);

			mesh.at(i) = cur_top_vertex;
			mesh.at(i + stacks) = cur_bottom_vertex;

			BuildNormalVisualizationGeometry();

		}
		theta = 0.0f;
		phi += increment_slices;

	}

	if (!this->PostGLInitialize(&this->vertex_array_handle, &this->vertex_coordinate_handle, this->vertices.size() * sizeof(VertexAttributesPCN), &this->vertices[0]))
		return false;

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttributesPCN), (GLvoid *) 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttributesPCN), (GLvoid *) (sizeof(vec3) ));	// Note offset - legacy of older code
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttributesPCN), (GLvoid *) (sizeof(vec3) * 2));	// Same
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
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


	if (!this->phong.Initialize("phong_shader.vert", "phong_shader.frag"))
		return false;

	if (!this->blue.Initialize("blue_shader.vert", "blue_shader.frag"))
		return false;

	if (!this->light.Initialize("light.vert", "light.frag"))
		return false;

	this->shaders.push_back(phong);
	this->shaders.push_back(blue);
	this->shaders.push_back(light);

	if (this->GLReturnedError("Sphere::Initialize - on exit"))
		return false;


	return true;
}

void Sphere::TakeDown()
{
	this->vertices.clear();
	this->phong.TakeDown();
	this->blue.TakeDown();
	this->light.TakeDown();
	this->adsShader.TakeDown();
	this->solid_color.TakeDown();
	super::TakeDown();
}

void Sphere::Draw(const ivec2 & size)
{
	assert(false);
}

void Sphere::Draw(const mat4 & projection, mat4 modelview, const ivec2 & size, GLint level, GLint shade, const float time)
{
	if (this->GLReturnedError("Sphere::Draw - on entry"))
		return;

	glEnable(GL_DEPTH_TEST);

	mat4 mvp = projection * modelview;
	mat3 nm = inverse(transpose(mat3(modelview)));

	this->shaders[this->shader_index].Use();
	this->shaders[this->shader_index].CommonSetup(time, value_ptr(size), value_ptr(projection), value_ptr(modelview), value_ptr(mvp), value_ptr(nm));

	if (this->shader_index == 2 && this->is_hit)
	{		
		light.SetLight(glm::vec4(0.0f, 0.0f, 2.0f, 1.0f), glm::vec3(0.2f), glm::vec3(0.7f), vec3(0.7f));
		light.SetMaterial(materials[MAT_RUBY].ambient, materials[MAT_RUBY].diffuse, materials[MAT_RUBY].specular, materials[MAT_RUBY].shiny);
		
	}
	if (this->shader_index == 2 && !this->is_hit)
	{		
		light.SetLight(glm::vec4(0.0f, 0.0f, 2.0f, 1.0f), glm::vec3(0.2f), glm::vec3(0.7f), vec3(0.7f));
		light.SetMaterial(materials[MAT_EMERALD].ambient, materials[MAT_EMERALD].diffuse, materials[MAT_EMERALD].specular, materials[MAT_EMERALD].shiny);
	}

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

	if (this->GLReturnedError("Sphere::Draw - on exit"))
		return;

	//time -= 1E3;
}


