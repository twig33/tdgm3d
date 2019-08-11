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
	//coords			//color
    -1.0,-1.0, 0.0, 	1.0, 0.0, 0.0,
     0.0, 0.0, 0.0, 	0.0, 1.0, 0.0,
     0.0, 0.5, 0.0, 	0.0, 0.0, 1.0,
	 1.0,-1.0, 0.0, 	1.0, 0.0, 0.0,
};  
unsigned int indices[] = {  // note that we start from 0!
        0, 1, 2,  // first Triangle 
		1, 2, 3   // second Triangle
};
float testvertices2[] = {
   -1.0,-1.0, 0.0,
	0.0, 0.0, 0.0,
	1.0,-1.0, 0.0,
};
unsigned int indices2[] = {
	0, 1, 2
};
float background_vertices[] = {
   -1.0,-1.0,-1.0,
    1.0, 1.0,-1.0,
   -1.0, 1.0,-1.0,
	1.0,-1.0,-1.0,
};
unsigned int background_indices[] = {
	0, 1, 2,
	1, 2, 3,
};
int main(int argc, char* argv[])
{
	Graphics.init();
	RenderObject* triangle = Graphics.new_triangles_object(	GRAPHICS_SHADER_COLOR_VERTEX, 
															sizeof(testvertices), testvertices, 
							  								sizeof(indices), indices);
	RenderObject* triangle2 = Graphics.new_triangles_object(GRAPHICS_SHADER_COLOR_SOLID,
															sizeof(testvertices2), testvertices2,
															sizeof(indices2), indices2,
															{0.0, 0.0, 1.0, 0.1});
	RenderObject* background = Graphics.new_triangles_object(GRAPHICS_SHADER_COLOR_SOLID,
	/*background is for testing alpha in colors*/			sizeof(background_vertices), background_vertices,
	/*alpha in colors doesnt work yet*/						sizeof(background_indices), background_indices,
															{1.0, 1.0, 1.0, 1.0});
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