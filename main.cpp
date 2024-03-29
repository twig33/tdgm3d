  #define GLEW_DLL
#define GLFW_DLL

#include <string>
#include <iostream>

#include <graphics.h>
#include <input.h>
#include <gamestate.h>
#include <gameobject.h>

#include <GLFW/glfw3.h>

double framerate = 1.0/60.0;
double multiplier = 1.0;

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
float testscalev[] = {
	1.0, 1.1, 0.0,
	2.0, 2.0, 0.0,
	3.0, 1.1, 0.0,
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
	GameObjects = new GameObjectManager();
	Input.init();
	GameState* gamestate;
	gamestate = new Game();
	gamestate->init();
	glfwSetKeyCallback(Graphics->window, Input.key_callback);
	VertexData* testscale = new VertexData(GRAPHICS_SHADER_COLOR_SOLID, sizeof(testscalev), testscalev,
										   								sizeof(indices2), indices2);
	VertexData* bg_vert = new VertexData(GRAPHICS_SHADER_COLOR_SOLID, sizeof(background_vertices), background_vertices,
										 								sizeof(background_indices), background_indices);
	RenderObject* bg = new RenderObject(bg_vert); Graphics->push(bg);
	bg->transform->set_position(glm::vec3(0.0f, 0.0f, -5.0f));
	bg->set_cycle_colors(true);
	//create vertex data
	VertexData* data2 = new VertexData(GRAPHICS_SHADER_COLOR_SOLID, sizeof(testvertices2), testvertices2,
									   								sizeof(indices2), indices2);
	//create an object
	RenderObject* object2 = new RenderObject(data2); Graphics->push(object2);
	object2->transform->set_position(glm::vec3(0.0f, 0.0f, -4.0f));
	object2->set_color(1.0f, 1.0f, 1.0f, 0.5f);
	//create 3 other objects with the same vertex data
	RenderObject* copy[3];
	copy[0] = new RenderObject(data2); Graphics->push(copy[0]);
	copy[1] = new RenderObject(data2); Graphics->push(copy[1]);
	copy[2] = new RenderObject(data2); Graphics->push(copy[2]);
	copy[0]->set_color(1.0f, 0.0f, 0.0f, 0.7f);
	copy[1]->set_color(0.0f, 1.0f, 0.0f, 0.3f);
	copy[2]->set_color(0.0f, 0.0f, 1.0f, 0.7f);
	copy[0]->transform->set_position(glm::vec3(-0.5, 0.0, -4.15f));
	copy[1]->transform->set_position(glm::vec3(1.0, 0.0,  -4.2f));
	copy[2]->transform->set_position(glm::vec3(1.5, 0.0,  -4.25f));
	RenderObject* testscaleobj = new RenderObject(testscale); Graphics->push(testscaleobj);
	testscaleobj->transform->set_position(glm::vec3(2.0, 0.0,  -4.25f));
	//initial movement speed
	glm::vec3 speed = glm::vec3(0.0f, 0.0f, 0.0f);
	int frames = 0;
	float color = 0.0f;
	float scale = 1.0f;
	Player player;
	GameObjects->push(&player);
	double temp_time = 0.0;
	while(!Graphics->quit){
		double time_before = glfwGetTime();
		//main
		int axis_x, axis_y, axis_z;
		if (Input.keys[GLFW_KEY_D] == Input.keys[GLFW_KEY_A]){
			axis_x = 0;	
		}
		else {
			axis_x = Input.keys[GLFW_KEY_D] ? 1 : -1;	
		}
		if (Input.keys[GLFW_KEY_W] == Input.keys[GLFW_KEY_S]){
			axis_y = 0;	
		}
		else {
			axis_y = Input.keys[GLFW_KEY_W] ? 1 : -1;	
		}
		if (Input.keys[GLFW_KEY_Q] == Input.keys[GLFW_KEY_E]){
			axis_z = 0;	
		}
		else {
			axis_z = Input.keys[GLFW_KEY_E] ? 1 : -1;	
		}
		speed.x = axis_x;
		speed.y = axis_y;
		speed.z = axis_z;
		scale += axis_z * 0.01f;
		Graphics->camera.translate(speed * -0.05f * (float)multiplier);
		testscaleobj->transform->translate(speed * 0.05f * (float)multiplier);
		testscaleobj->transform->set_scale(scale);
		for (int i = 0; i < 3; ++i){
			copy[i]->transform->translate(speed * 0.05f * (float)multiplier);
		}
		temp_time = glfwGetTime();
		gamestate->update();
		std::cout << "gamestate update took " << glfwGetTime() - temp_time << "\n";
		temp_time = glfwGetTime();
		GameObjects->update();
		std::cout << "gameobjects update took " << glfwGetTime() - temp_time << "\n";
		temp_time = glfwGetTime();
		Graphics->update(); 
		std::cout << "graphics update took " << glfwGetTime() - temp_time << "\n";
		//main
		double time_after = glfwGetTime();
		double difference = time_after - time_before;
		if (difference < framerate){
			while (glfwGetTime() < time_before + framerate){}
			if (multiplier != 1){
				multiplier = 1;	
			}
			std::cout << difference << "\n";
		}else{
			multiplier = difference / framerate;
			std::cout << "Slowdown!\n";
		}
	}
}