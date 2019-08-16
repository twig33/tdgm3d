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
   -1.0,-0.9, 0.0,
	0.0, 0.0, 0.0,
	1.0,-0.9, 0.0,
};
unsigned int indices2[] = {
	0, 1, 2
};
float background_vertices[] = {
	//coords			//color
   -3.0, 3.0, 0.0,		0.0, 0.0, 1.0,
    3.0, 3.0, 0.0,		0.0, 0.0, 1.0,
   -3.0,-3.0, 0.0,		1.0, 0.0, 0.0,
	3.0,-3.0, 0.0,		1.0, 0.0, 0.0
};
unsigned int background_indices[] = {
	0, 1, 2,
	1, 2, 3,
};
int main(int argc, char* argv[])
{
	Graphics = new GraphicsManager();
	RenderObject* triangle = Graphics->new_triangles_object(	GRAPHICS_SHADER_COLOR_VERTEX, 
															sizeof(testvertices), testvertices, 
							  								sizeof(indices), indices);
	triangle->set_position(glm::vec3(0.0,0.0,-3.5f));
	RenderObject* triangle2 = Graphics->new_triangles_object(GRAPHICS_SHADER_COLOR_SOLID_TRANSPARENT,
															sizeof(testvertices2), testvertices2,
															sizeof(indices2), indices2,
															{0.0, 0.0, 1.0, 0.5});
	triangle2->set_position(glm::vec3(0.0,0.0,-3.5f));
	RenderObject* copy[3];
	glm::vec3 speed[3];
	speed[0] = glm::vec3(1.0f, 0.7f, 0.0f);
	speed[1] = glm::vec3(-1.3f, 1.0f, 0.0f);
	speed[2] = glm::vec3(1.0f, -1.5f, 0.0f);
	copy[0] = Graphics->new_copy_object(triangle2);
	copy[1] = Graphics->new_copy_object(triangle2);
	copy[2] = Graphics->new_copy_object(triangle2);
	copy[0]->set_color(1.0f, 0.0f, 0.0f, 0.5f);
	copy[1]->set_color(0.0f, 1.0f, 0.0f, 0.5f);
	copy[2]->set_color(0.0f, 0.0f, 1.0f, 0.5f);
	copy[0]->set_position(glm::vec3(-0.5, 0.0, -4.15f));
	copy[1]->set_position(glm::vec3(1.0, 0.0, -4.1f));
	copy[2]->set_position(glm::vec3(1.5, 0.0, -4.05f));
	RenderObject* background = Graphics->new_triangles_object(GRAPHICS_SHADER_COLOR_VERTEX,
	/*i made background to test depth*/						sizeof(background_vertices), background_vertices,
															sizeof(background_indices), background_indices);
	background->set_position(glm::vec3(0.0,0.0,-4.2f));
	//background->set_scale(glm::vec3(20.0,20.0,1.0));
	Input.init();
	glfwSetKeyCallback(Graphics->window, Input.key_callback);
	int frames = 0;
	while(!Graphics->quit){
		double time_before = glfwGetTime();
		//main
		for (int i = 0; i < 3; ++i){
			copy[i]->translate(speed[i] * 0.05f * (float)multiplier);
			if (copy[i]->position.x + copy[i]->bounds_left < -5.5f){
					speed[i].x *= -1;
			}
			if (copy[i]->position.x + copy[i]->bounds_right > 5.5f){
					speed[i].x *= -1;
			}
			if (copy[i]->position.y + copy[i]->bounds_up > 4.0f){
					speed[i].y *= -1;
			}
			if (copy[i]->position.y + copy[i]->bounds_down < -4.0f){
					speed[i].y *= -1;
			}
		}
		Graphics->update();
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