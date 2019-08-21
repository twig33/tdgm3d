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

class GraphicsManager {
	public:
		GraphicsManager();
		void finish();
		void update();
		void push(RenderObject* object);
		void remove(RenderObject* object);
		bool quit = 0;
		GLFWwindow* window;
	private:
		glm::mat4 proj;
		glm::mat4 camera;
};

extern GraphicsManager* Graphics;
#endif