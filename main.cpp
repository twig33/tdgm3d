  #define GLEW_DLL
#define GLFW_DLL

#include <string>
#include <iostream>

#include <graphics.h>
#include <input.h>

#include <GLFW/glfw3.h>

double framerate = 1/60;
double multiplier = 1;

float testvertices[] = {
    -1.0f, -1.0,  0.0f,
     0.0f, 0.0f, 0.0f,
     0.0f,  0.5f, 0.0f,
	 1.0, -1.0, 0.0
};  
unsigned int indices[] = {  // note that we start from 0!
        0, 1, 2,  // first Triangle 
		1, 2, 3   // second Triangle
};
int main(int argc, char* argv[])
{
	Graphics.init();
	RenderObject* triangle = Graphics.new_triangles_object(GRAPHICS_COLOR_SOLID, sizeof(testvertices), testvertices,
							  							 sizeof(indices),indices);
	Input.init();
	glfwSetKeyCallback(Graphics.window, Input.key_callback);
	int frames = 0;
	while(!Graphics.quit){
		double time_before = glfwGetTime();
		//main
		Graphics.update();
		//main
		double time_after = glfwGetTime();
		double difference = time_before - time_after;
		if (difference < framerate){
			while (glfwGetTime() < time_after + framerate - difference){}
			if (multiplier != 1){
				multiplier = 1;	
			}
		}else{
			multiplier = difference / framerate;	
		}
	}
}