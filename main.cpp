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
   -3.0, 3.0, 0.0,//		0.0, 0.0, 1.0,
    3.0, 3.0, 0.0,//		0.0, 0.0, 1.0,
   -3.0,-3.0, 0.0,//		1.0, 0.0, 0.0,
	3.0,-3.0, 0.0,//	1.0, 0.0, 0.0
};
unsigned int background_indices[] = {
	0, 1, 2,
	1, 2, 3,
};
int main(int argc, char* argv[])
{
	Graphics = new GraphicsManager();
	Input.init();
	glfwSetKeyCallback(Graphics->window, Input.key_callback);
	VertexData* bg_vert = new VertexData(GRAPHICS_SHADER_COLOR_SOLID, sizeof(background_vertices), background_vertices,
										 								sizeof(background_indices), background_indices);
	RenderObject* bg = new RenderObject(bg_vert); Graphics->push(bg);
	bg->transform.set_position(glm::vec3(0.0f, 0.0f, -5.0f));
	bg->set_cycle_colors(true);
	//create vertex data
	VertexData* data2 = new VertexData(GRAPHICS_SHADER_COLOR_SOLID, sizeof(testvertices2), testvertices2,
									   								sizeof(indices2), indices2);
	//create an object
	RenderObject* object2 = new RenderObject(data2); Graphics->push(object2);
	object2->transform.set_position(glm::vec3(0.0f, 0.0f, -4.0f));
	object2->set_color(1.0f, 1.0f, 1.0f, 0.5f);
	//create 3 other objects with the same vertex data
	RenderObject* copy[3];
	copy[0] = new RenderObject(data2); Graphics->push(copy[0]);
	copy[1] = new RenderObject(data2); Graphics->push(copy[1]);
	copy[2] = new RenderObject(data2); Graphics->push(copy[2]);
	copy[0]->set_color(1.0f, 0.0f, 0.0f, 0.7f);
	copy[1]->set_color(0.0f, 1.0f, 0.0f, 0.3f);
	copy[2]->set_color(0.0f, 0.0f, 1.0f, 0.7f);
	copy[0]->transform.set_position(glm::vec3(-0.5, 0.0, -4.15f));
	copy[1]->transform.set_position(glm::vec3(1.0, 0.0, -4.1f));
	copy[2]->transform.set_position(glm::vec3(1.5, 0.0, -4.05f));
	//initial movement speed
	glm::vec3 speed[3];
	speed[0] = glm::vec3(0.0f, 0.0f, -1.0f);
	speed[1] = glm::vec3(0.0f, 0.0f, 0.7f);
	speed[2] = glm::vec3(0.0f,  0.0f, 1.2f);
	int frames = 0;
	float color = 0.0f;
	while(!Graphics->quit){
		double time_before = glfwGetTime();
		//main
		for (int i = 0; i < 3; ++i){
			copy[i]->transform.translate(speed[i] * 0.05f * (float)multiplier);
			if (copy[i]->transform.get_position().x + copy[i]->vertex_data->get_bounds_left() < -5.5f){
					speed[i].x *= -1;
			}
			if (copy[i]->transform.get_position().x + copy[i]->vertex_data->get_bounds_right() > 5.5f){
					speed[i].x *= -1;
			}
			if (copy[i]->transform.get_position().y + copy[i]->vertex_data->get_bounds_up() > 4.0f){
					speed[i].y *= -1;
			}
			if (copy[i]->transform.get_position().y + copy[i]->vertex_data->get_bounds_down() < -4.0f){
					speed[i].y *= -1;
			}
			if (copy[i]->transform.get_position().z < -4.95f){
					speed[i].z *= -1;
			}
			if (copy[i]->transform.get_position().z > -4.0f){
					speed[i].z *= -1;
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