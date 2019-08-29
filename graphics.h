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
#include <renderobject.h>

enum { //resources
	GRAPHICS_RESOURCE_SPHERE,
	GRAPHICS_RESOURCE_CUBE,
	GRAPHICS_RESOURCE_STAR_DESTROYER,
	GRAPHICS_RESOURCE_BOMB,
	GRAPHICS_RESOURCE_SIZE
};

class GraphicsManager {
	public:
		GraphicsManager();
		void finish();
		void update();
		void push(RenderObject* object);
		void remove(RenderObject* object);
		VertexData* get_vertex_data(unsigned int id);
		bool quit = 0;
		GLFWwindow* window;
		Transform camera; //everyone can move the camera
	private:
		const char* resource_paths [GRAPHICS_RESOURCE_SIZE] = {
			resource_paths[GRAPHICS_RESOURCE_SPHERE] = "sphere.obj",
			resource_paths[GRAPHICS_RESOURCE_CUBE] = "cube.obj",
			resource_paths[GRAPHICS_RESOURCE_STAR_DESTROYER] = "star_destroyer.obj",
			resource_paths[GRAPHICS_RESOURCE_BOMB] = "bomb.obj",
		};
		VertexData* resources_vertex_data[GRAPHICS_RESOURCE_SIZE];
		void load_resources();
		void process_obj(const char* path, std::size_t* size_vertices, float** vertices, std::size_t* size_indices, unsigned int** indices);  
		glm::mat4 proj;
};

extern GraphicsManager* Graphics;
#endif