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

struct Color {
	float r = 0;
	float g = 0;
	float b = 0;
	float a = 1;
};
struct Vector3{
	float x = 0;
	float y = 0;
	float z = 0;
};
enum { //GRAPHICS_SHADER_SIZE is always last
	GRAPHICS_SHADER_COLOR_SOLID, GRAPHICS_SHADER_COLOR_VERTEX, GRAPHICS_SHADER_SIZE
};
enum {
	GRAPHICS_OBJECT_STATE_ALIVE, GRAPHICS_OBJECT_STATE_DEAD, GRAPHICS_OBJECT_STATE_EXPLODING, GRAPHICS_OBJECT_STATE_SIZE
};
struct RenderObject {
	friend class GraphicsManager;
	private:
		unsigned int state = GRAPHICS_OBJECT_STATE_ALIVE;
		Color color;
		RenderObject* next = NULL;
		Vector3 position;
		Vector3 rotation;
		unsigned int VBO;
		unsigned int EBO;
		unsigned int VAO;
		unsigned int vertices_count = 0;
};

class GraphicsManager {
	public:
		void init();
		void finish();
		void update();
		RenderObject* new_triangles_object(	int shader_type, 
											std::size_t size_vertices, float vertices[],
							  				std::size_t size_indices, unsigned int indices[],
										  	Color color = {0.0, 0.0, 0.0, 1.0});
		bool quit = 0;
		GLFWwindow* window;
	private:
		RenderObject* tail[GRAPHICS_SHADER_SIZE] = {NULL};
		RenderObject* head[GRAPHICS_SHADER_SIZE] = {NULL};
		RenderObject* new_empty_object_push_back(int shader_type);
		unsigned int shader_program[GRAPHICS_SHADER_SIZE];
		typedef void (GraphicsManager::*void_func)(RenderObject*);
		void solid_color_shader_actions(RenderObject* object);
		void vertex_color_shader_actions(RenderObject* object);
		void_func shader_specific_actions[GRAPHICS_SHADER_SIZE] = {
			shader_specific_actions[GRAPHICS_SHADER_COLOR_SOLID] = 	&GraphicsManager::solid_color_shader_actions,
			shader_specific_actions[GRAPHICS_SHADER_COLOR_VERTEX] = &GraphicsManager::vertex_color_shader_actions
		};
		void do_shader_specific_actions(int shader_type, RenderObject* object);
		int shader_color_solid_ourColor; //color uniform location in solid color shader (so we can change the color)
};

extern GraphicsManager Graphics;
#endif