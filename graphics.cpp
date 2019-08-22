#include <graphics.h>
#include <rendergroup.h>
#include <iostream>

GraphicsManager* Graphics;

GraphicsManager::GraphicsManager(){
	//glfw init
	glfwInit();
	//window
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	window = glfwCreateWindow(640, 480, "tdgm3d", nullptr,
	nullptr);
	glfwMakeContextCurrent(window);
	//viewport
	glViewport(0, 0, 640, 480);
	//glew init 
	glewExperimental = GL_TRUE; //ya uzhe забылdlya chevo eto
	glewInit();
	glEnable(GL_DEPTH_TEST); 
	glEnable(GL_BLEND); 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
	//create shaders
	proj = glm::perspective(glm::radians(90.0f), (float)640/(float)480, 0.1f, 100.0f);
	camera = glm::mat4(1.0f);
	//set screen clear color
	glClearColor(0.9, 0.9, 1, 1);
	Shaders = new ShaderManager();
	RenderGroups = new RenderGroupManager();
	RenderGroups->set_projection(proj);
	RenderGroups->set_view(camera);
}

void GraphicsManager::finish(){
	this->quit = 1;
	glfwTerminate();	
}
void GraphicsManager::push(RenderObject* object){
	RenderGroups->push(object);	
}
void GraphicsManager::remove(RenderObject* object){
	RenderGroups->remove(object);	
}
void GraphicsManager::update(){
	if (glfwWindowShouldClose(window)){
		this->finish();
		return;	
	}
	RenderGroups->draw();
	glfwSwapBuffers(window);
	glfwPollEvents();
}