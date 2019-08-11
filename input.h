#ifndef INPUT_H
#define INPUT_H
#include <GLFW/glfw3.h>

class InputManager{
	public:
		bool keys[256];
		static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
		void init();
};

extern InputManager Input;
#endif