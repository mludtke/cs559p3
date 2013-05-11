/*****************************************************
*	Sphere
*
*	Spherical object with an assortment of shaders.
*****************************************************/


#include <iostream>
#include "Sphere.h"

using namespace std;
using namespace glm;

Sphere::Sphere() : Object()
{
	this->shader_index = 0;
	this->material_index = 0;
	this->light_index = 0;
	this->lightPos[0] = vec4(0.0f);
	//http://www.johndcook.com/cpp_TR1_random.html
	/*std::tr1::mt19937 eng;
	std::tr1::uniform_int<int> unif(-5, 5);
	for (int i = 0; i < 5; ++i)
	std::cout << "unif: " << unif(eng) << std::endl;*/
	for(int i = 1; i < 10; i++)	
	{
		this->lightPos[i] = vec4(i, 0.0f, 0.0f, 1.0f);
		//cout << to_string(lightPos[i]) << endl;
	}
	this->x = this->y = this->z = 0.0f;
	/*cout << "shader_index: " << shader_index << endl;
	cout << "shaders.size(): " << shaders.size() << endl;*/
}

void Sphere::StepShader()
{
	assert (this->shaders.size() != 0);
	
	this->shader_index = ++this->shader_index % this->shaders.size();	
	cout << "shader: " << shader_index << endl;
}

void Sphere::StepMaterial()
{
	this->material_index = ++this->material_index % 19;
	if (material_index == 15)
		material_index = 16;
	if (material_index == 17)
		material_index = 0;
	cout << "material: " << material_index << endl;
}

void Sphere::StepLightPos()
{
	cout << randomLightPosition() << endl;
	this->light_index = ++this->light_index % 10;  
	cout << "this->light_index: " << this->light_index << endl;
	//cout << to_string(lightPos[light_index]) << endl;
}

void Sphere::incX()
{
	this->x++;
}

void Sphere::incY()
{
	this->y++;
}

void Sphere::incZ()
{
	this->z++;
}

void Sphere::decX()
{
	this->x--;
}

void Sphere::decY()
{
	this->y--;
}

void Sphere::decZ()
{
	this->z--;
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

void Sphere::hit(float time, float born_time)
{
	if(!is_hit)
	{
		this->is_hit = true;
	}
	this->time = 0.0f;
	this->end_time = time;
	this->born_time = born_time;
	//cout << "time: " << time << " Born time: " << born_time;
}

bool Sphere::is_sphere_hit()
{
	if (is_hit == 1)
		return true;
	return false;
}

float Sphere::getTime()
{
	return this->time;
}

void Sphere::setTime(float t)
{
	this->time = t;
	if (t > this->end_time)
	{
		this->is_hit = false;
		this->end_time = 0.0f;
	}
}

void Sphere::reset()
{
	this->time = 0.0f;
	this->is_hit = 0;
	this->born_time = 0.0f;
	this->end_time = 0.0f;
}

float Sphere::getBirthTime()
{
	return this->born_time;
}

float Sphere::getEndTime()
{
	return this->end_time;
}

vec3 Sphere::getPostion()
{
	return this->position;
}

void Sphere::setPosition(vec3 pos)
{
	position = pos;
}

bool Sphere::Initialize(int hit, GLfloat radius, GLint slices, GLint stacks )
{
	if (this->GLReturnedError("Sphere::Initialize - on entry"))
		return false;

	if (!super::Initialize())
		return false;

	vec3 color;
	if (hit == 0)
	{
		color = GREEN;
		is_hit = 0;
	}
	else if (hit == 1)
	{
		color = RED;
		is_hit = 1;
	}
	else if (hit == 2)
	{
		color = BLUE;
		is_hit = 2;
	}
	else if (hit == 3)
	{
		color = GRAY;
		is_hit = 3;
	}

	GLfloat del = 10.0f;
	//  p1 = p2
	//  ||   ||
	//  p0 = p3
	GLfloat x0,y0,z0, x1,y1,z1, x2,y2,z2, x3,y3,z3;
	VertexAttributesPCNT cur_bot_vertex, cur_top_vertex, nxt_top_vertex, nxt_bot_vertex;
	// http://csclab.murraystate.edu/bob.pilgrim/515/texture_maps_project.html
	for (GLfloat verAng = 0; verAng < 180.0f; verAng += del) // stacks
	{
		y0 = y3 = radius * cos(radians(verAng));
		y1 = y2 = radius * cos(radians(verAng + del));
		for (GLfloat horAng = 0; horAng < 360.0f; horAng += del) //slices
		{
			x0 = radius * cos(radians(horAng)) * sin(radians(verAng));
			x1 = radius * cos(radians(horAng)) * sin(radians(verAng + del));
			x2 = radius * cos(radians(horAng + del)) * sin(radians(verAng + del));
			x3 = radius * cos(radians(horAng + del)) * sin(radians(verAng));
			z0 = radius * sin(radians(horAng)) * sin(radians(verAng));
			z1 = radius * sin(radians(horAng)) * sin(radians(verAng + del));
			z2 = radius * sin(radians(horAng + del)) * sin(radians(verAng + del));
			z3 = radius * sin(radians(horAng + del)) * sin(radians(verAng));

			cur_bot_vertex.position = vec3(x0,y0,z0);
			cur_top_vertex.position = vec3(x1,y1,z1);
			nxt_top_vertex.position = vec3(x2,y2,z2);
			nxt_bot_vertex.position = vec3(x3,y3,z3);

			cur_bot_vertex.color = color;
			cur_top_vertex.color = color;
			nxt_top_vertex.color = color;
			nxt_bot_vertex.color = color;

			cur_bot_vertex.normal = cur_bot_vertex.position;
			cur_top_vertex.normal = cur_top_vertex.position;
			nxt_top_vertex.normal = nxt_top_vertex.position;
			nxt_bot_vertex.normal = nxt_bot_vertex.position;

			cur_bot_vertex.texture_coordinate = vec2(horAng/360.0f,verAng/180.0f);
			cur_top_vertex.texture_coordinate = vec2(horAng/360.0f,(verAng+del)/180.0f);
			nxt_top_vertex.texture_coordinate = vec2((horAng+del)/360.0f,(verAng+del)/180.0f);
			nxt_bot_vertex.texture_coordinate = vec2((horAng+del)/360.0f,verAng/180.0f);

			this->vertices.push_back(cur_bot_vertex);
			this->vertices.push_back(cur_top_vertex);
			this->vertices.push_back(nxt_top_vertex);
			this->vertices.push_back(nxt_bot_vertex);

			this->vertex_indices.push_back(this->vertices.size() - 4);
			this->vertex_indices.push_back(this->vertices.size() - 2);
			this->vertex_indices.push_back(this->vertices.size() - 3);

			this->vertex_indices.push_back(this->vertices.size() - 4);
			this->vertex_indices.push_back(this->vertices.size() - 1);
			this->vertex_indices.push_back(this->vertices.size() - 2);

			this->BuildNormalVisualizationGeometry();

			x0 = x1;
			z0 = z1;
		}    
	}  

	if (!this->PostGLInitialize(&this->vertex_array_handle, &this->vertex_coordinate_handle, this->vertices.size() * sizeof(VertexAttributesPCNT), &this->vertices[0]))
		return false;

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttributesPCNT), (GLvoid *) 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttributesPCNT), (GLvoid *) (sizeof(vec3) ));	
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

	assert(this->texture.Initialize("./shaders/basic_texture_shader.vert", "./shaders/basic_texture_shader.frag"));
	assert(this->phong.Initialize("./shaders/phong_shader.vert", "./shaders/phong_shader.frag"));
	
	assert(this->phong2.Initialize("./shaders/phong2.vert", "./shaders/phong2.frag"));
	//assert(this->twoside.Initialize("twoside.vert", "twoside.frag"));
	assert(this->flat.Initialize("./shaders/flat.vert", "./shaders/flat.frag"));
	assert(this->perforated.Initialize("./shaders/perforated.vert", "./shaders/perforated.frag"));
	assert(this->time_perforated.Initialize("./shaders/time_perforated.vert", "./shaders/time_perforated.frag"));  
	//assert(this->normal.Initialize("normal.vert", "normal.frag"));
	assert(this->dark.Initialize("./shaders/solid_shader.vert", "./shaders/solid_shader.frag"));  
	assert(this->toon.Initialize("./shaders/toon.vert", "./shaders/toon.frag"));

	this->shaders.push_back(&phong);
	this->shaders.push_back(&texture);
	this->shaders.push_back(&perforated);
	this->shaders.push_back(&time_perforated);
	this->shaders.push_back(&phong2);
	//this->shaders.push_back(&twoside);
	this->shaders.push_back(&flat);
	this->shaders.push_back(&dark);
	this->shaders.push_back(&toon);
	

	if (this->GLReturnedError("Sphere::Initialize - on exit"))
		return false;	

	return true;
}

void Sphere::TakeDown()
{
	this->vertices.clear();
	this->shaders.clear();
	this->shader.TakeDown();	
	this->phong.TakeDown();	
	this->phong2.TakeDown();	
	this->twoside.TakeDown();	
	this->flat.TakeDown();
	this->perforated.TakeDown();
	this->texture.TakeDown();
	this->normal.TakeDown();
	this->toon.TakeDown();
	super::TakeDown();
}


void Sphere::Draw(const ivec2 & size)
{
	assert(false);
}

void Sphere::Draw(const mat4 & projection, mat4 modelview, const ivec2 & size, GLint level, GLint shader, const float time)
{
	if (this->GLReturnedError("Sphere::Draw - on entry"))
		return;

	glEnable(GL_DEPTH_TEST);

	mat4 mvp = projection * modelview;
	mat3 nm = inverse(transpose(mat3(modelview)));

	TextureManager::Inst()->BindTexture(15, 0); //green texture
	if (is_hit == 1)
		TextureManager::Inst()->BindTexture(16, 0); //red texture
	if (is_hit == 3)
		TextureManager::Inst()->BindTexture(17, 0); //bomb texture
	if (is_hit == 2)
		TextureManager::Inst()->BindTexture(18, 0); //player texture

	glTexEnvf(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_REPLACE);
	glTexParameterf(GL_TEXTURE_2D , GL_TEXTURE_WRAP_S , GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D , GL_TEXTURE_WRAP_T , GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_LINEAR);

	/*phong.Use();
	phong.CommonSetup(time, value_ptr(size), value_ptr(projection), value_ptr(modelview), value_ptr(mvp), value_ptr(nm));  */

	this->shaders[this->shader_index]->Use();
	this->shaders[this->shader_index]->CommonSetup(time, value_ptr(size), value_ptr(projection), value_ptr(modelview), value_ptr(mvp), value_ptr(nm));  

	if (this->shader_index > 1 && this->shader_index < (shaders.size() - 2))
	{
		((ADSShader *)this->shaders[this->shader_index])->SetLight(vec4(x,y,z,1.0f), vec3(0.2f), vec3(0.7f), vec3(0.7f));
		((ADSShader *)this->shaders[this->shader_index])->SetMaterial(materials[this->material_index].ambient, materials[this->material_index].diffuse, 
			materials[this->material_index].specular, materials[this->material_index].shiny);	

		if (is_hit == 1) //hit is red color (ruby)
		{
			((ADSShader *)this->shaders[this->shader_index])->SetMaterial(materials[MAT_RUBY].ambient, materials[MAT_RUBY].diffuse, 
				materials[MAT_RUBY].specular, materials[MAT_RUBY].shiny);	
		}

		if (is_hit == 3) // Bomb is black color
		{
			((ADSShader *)this->shaders[this->shader_index])->SetMaterial(materials[MAT_BLACK_RUBBER].ambient, materials[MAT_BLACK_RUBBER].diffuse, 
				materials[MAT_BLACK_RUBBER].specular, materials[MAT_BLACK_RUBBER].shiny);	
		}
	} 

	if (this->shader_index == shaders.size() - 1)
	{
		((ToonShader *)this->shaders[this->shader_index])->SetLight(vec4(x,y,z,1.0f), vec3(0.2f));
		((ToonShader *)this->shaders[this->shader_index])->SetMaterial(materials[this->material_index].ambient, materials[this->material_index].diffuse);	

		if (is_hit == 1) //hit is red color (ruby)
		{
			((ToonShader *)this->shaders[this->shader_index])->SetMaterial(materials[MAT_RUBY].ambient, materials[MAT_RUBY].diffuse);	
		}

		if (is_hit == 3) // Bomb is black color
		{
			((ToonShader *)this->shaders[this->shader_index])->SetMaterial(materials[MAT_BLACK_RUBBER].ambient, materials[MAT_BLACK_RUBBER].diffuse);	
		}
	}

	glBindVertexArray(this->vertex_array_handle);
	glDrawElements(GL_TRIANGLES , this->vertex_indices.size(), GL_UNSIGNED_INT , &this->vertex_indices[0]);
	glBindVertexArray(0);
	glUseProgram(0);

	if (this->draw_normals)
	{
		this->normal.Use();
		this->normal.CommonSetup(time, value_ptr(size), value_ptr(projection), value_ptr(modelview), value_ptr(mvp), value_ptr(nm));
		glBindVertexArray(this->normal_array_handle);
		glDrawElements(GL_LINES , this->normal_indices.size(), GL_UNSIGNED_INT , &this->normal_indices[0]);
		glBindVertexArray(0);
		glUseProgram(0);
	}

	if (this->GLReturnedError("Sphere::Draw - on exit"))
		return;

}


