#include <input.h>

InputManager Input;

void InputManager::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
	if (action == GLFW_REPEAT)
		return;
	int mode = (action == GLFW_PRESS);
	Input.keys[key] = (bool)mode;
}

void InputManager::init(){
	
}