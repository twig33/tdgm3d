#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
#include <stb_image.h>
#include <shader.h>

struct Color {
	float r = 0;
	float g = 0;
	float b = 0;
	float a = 1;
};
enum { //GRAPHICS_SHADER_SIZE is always last
	GRAPHICS_SHADER_COLOR_SOLID, 
	GRAPHICS_SHADER_COLOR_VERTEX,
	GRAPHICS_SHADER_COLOR_SOLID_TRANSPARENT, 
	GRAPHICS_SHADER_SIZE
};
enum {
	GRAPHICS_OBJECT_STATE_ALIVE, GRAPHICS_OBJECT_STATE_DEAD, GRAPHICS_OBJECT_STATE_EXPLODING, GRAPHICS_OBJECT_STATE_SIZE
};
class RenderObject {
	friend class GraphicsManager;
	public:
		~RenderObject();
		void set_position(const glm::vec3 &pos);
		void set_rotation(const glm::vec3 &rot);
		void set_scale(const glm::vec3 &scalein);
		glm::vec3 position = glm::vec3(0.0, 0.0, 0.0);
	private:
		unsigned int state = GRAPHICS_OBJECT_STATE_ALIVE;
		unsigned int shader_type;
		Color color;
		RenderObject* previous = NULL;
		RenderObject* next = NULL;
		glm::mat4 transform = glm::mat4(1.0f);
		glm::mat4 rotation_mat = glm::mat4(1.0f);
		glm::mat4 position_mat = glm::mat4(1.0f);
		glm::mat4 scale_mat = glm::mat4(1.0f);
		void update_transform_matrix();
		unsigned int VBO;
		unsigned int EBO;
		unsigned int VAO;
		unsigned int vertices_count = 0;
};

class GraphicsManager {
	public:
		GraphicsManager();
		void finish();
		void update();
		RenderObject* new_triangles_object(	int shader_type, 
											std::size_t size_vertices, float vertices[],
							  				std::size_t size_indices, unsigned int indices[],
										  	Color color = {0.0, 0.0, 0.0, 1.0});
		void remove_object(RenderObject* object, bool destroy = true);
		void insert_by_z(RenderObject* object);
		bool quit = 0;
		GLFWwindow* window;
	private:
		glm::mat4 proj;
		glm::mat4 camera;
		glm::mat4 transform;
		RenderObject* tail[GRAPHICS_SHADER_SIZE] = {NULL}; //doubly linked list
		RenderObject* head[GRAPHICS_SHADER_SIZE] = {NULL}; //doubly linked list
		RenderObject* new_empty_object_push_back(int shader_type);
		Shader* shader[GRAPHICS_SHADER_SIZE]; //shaders storage
		typedef void (GraphicsManager::*void_func)(RenderObject*); //shader specific action function pointer typedef
		void solid_color_shader_actions(RenderObject* object); 
		void vertex_color_shader_actions(RenderObject* object);
		void_func shader_specific_actions[GRAPHICS_SHADER_SIZE] = { //shader specific action function pointers storage
			shader_specific_actions[GRAPHICS_SHADER_COLOR_SOLID] = 	&GraphicsManager::solid_color_shader_actions,
			shader_specific_actions[GRAPHICS_SHADER_COLOR_VERTEX] = &GraphicsManager::vertex_color_shader_actions,
			shader_specific_actions[GRAPHICS_SHADER_COLOR_SOLID_TRANSPARENT] = 	&GraphicsManager::solid_color_shader_actions
		};
		void do_shader_specific_actions(int shader_type, RenderObject* object); //wrapper because using function pointers is ugly
};

extern GraphicsManager* Graphics;
#endif