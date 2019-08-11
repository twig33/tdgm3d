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
	float r = 1;
	float g = 1;
	float b = 1;
};
struct Vector3{
	float x = 0;
	float y = 0;
	float z = 0;
};
enum {
	GRAPHICS_COLOR_SOLID, GRAPHICS_COLOR_VERTIX	
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
		RenderObject* new_triangles_object(	int color_type, 
											std::size_t size_vertices, float vertices[],
							  				std::size_t size_indices, unsigned int indices[]);
		bool quit = 0;
		GLFWwindow* window;
		RenderObject* tail = NULL;
		RenderObject* head = NULL;
	private:
		RenderObject* new_empty_object_push_back();
		unsigned int shader_program;
};

extern GraphicsManager Graphics;
#endif