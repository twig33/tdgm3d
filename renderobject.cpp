#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <shader.h>
#include <renderobject.h>

glm::vec3 Transform::get_position(){
	return position;	
}
const float* Transform::get_transform_mat_value_ptr() const {
	return glm::value_ptr(transform);
}
void Transform::set_position(const glm::vec3& pos){
	position_mat = glm::translate(glm::mat4(1.0f), pos);
	position = pos;
	update_transform_matrix();
}
void Transform::translate(const glm::vec3& tr){
	position_mat = glm::translate(position_mat, tr);
	position += tr;
	update_transform_matrix();
}
void Transform::set_rotation(const glm::vec3& rot){
	rotation_mat = glm::rotate(glm::mat4(1.0f), glm::radians(rot.x), glm::vec3(1.0, 0.0, 0.0));
	rotation_mat = glm::rotate(rotation_mat, glm::radians(rot.y), glm::vec3(0.0, 1.0, 0.0));
	rotation_mat = glm::rotate(rotation_mat, glm::radians(rot.z), glm::vec3(0.0, 0.0, 1.0));
	update_transform_matrix();
}
void Transform::set_scale(const glm::vec3& scalein){
	scale_mat = glm::scale(glm::mat4(1.0f), scalein);
	update_transform_matrix();
}
void Transform::update_transform_matrix(){
	transform = scale_mat * position_mat * rotation_mat;
}
VertexData::VertexData(unsigned int st, std::size_t size_vertices, float vertices[],
										std::size_t size_indices, unsigned int indices[])
	: shader_type(st) {
	if (size_indices % 3){
		std::cout << "Warning: indices size not a multiply of 3\n";	
	}
	vertices_count = size_indices / sizeof(unsigned int);
	bounds_left = vertices[0];
	bounds_right = vertices[0];
	bounds_up = vertices[1];
	bounds_down = vertices[1];
	for (int i = 0; i < vertices_count * Shaders->get_shader(shader_type)->stride; i += Shaders->get_shader(shader_type)->stride){
		if (bounds_left > vertices[i]){
			bounds_left = vertices[i];	
		}
		if (bounds_right < vertices[i]){
			bounds_right = vertices[i];	
		}
		if (bounds_up < vertices[i+1]){
			bounds_up = vertices[i+1];	
		}
		if (bounds_down > vertices[i+1]){
			bounds_down = vertices[i+1];	
		}
	}
	//generate vbo ebo vao
	glGenBuffers(1, &(VBO));
	glGenBuffers(1, &(EBO));
	glGenVertexArrays(1, &(VAO));
	//bind vao
	glBindVertexArray(VAO);
	//fill data
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, size_vertices, vertices, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size_indices, indices, GL_DYNAMIC_DRAW);
	//attributes are shader specific
	Shaders->get_shader(st)->bind_attributes();
	//cleanup unbind all
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
VertexData::~VertexData(){
	glDeleteBuffers(1, &(VBO));
	glDeleteBuffers(1, &(EBO));
	glDeleteVertexArrays(1, &(VAO));
}
void RenderObject::set_color(const float r, const float g, const float b, const float a){
	if (vertex_data->shader_type != GRAPHICS_SHADER_COLOR_SOLID){
		std::cout << "Setting the color won't have an effect if the shader isnt solidcolor\n";
		return;
	}
	color = {r >= 0 && r <= 1 ? r : color.r,
			 g >= 0 && g <= 1 ? g : color.g,
			 b >= 0 && b <= 1 ? b : color.b,
			 a >= 0 && a <= 1 ? a : color.a};
}
Color RenderObject::get_color(){
	return color;	
}
RenderObject::~RenderObject(){
	delete vertex_data;	
}