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
struct RenderObject {
	friend class GraphicsManager;
	private:
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
		RenderObject* tail[GRAPHICS_SHADER_SIZE] = {NULL};
		RenderObject* head[GRAPHICS_SHADER_SIZE] = {NULL};
	private:
		RenderObject* new_empty_object_push_back(int shader_type);
		unsigned int shader_program[GRAPHICS_SHADER_SIZE];
		int shader_color_solid_ourColor; //color uniform location in solid color shader (so we can change the color)
};

extern GraphicsManager Graphics;
#endif