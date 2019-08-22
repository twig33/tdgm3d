#include <graphics.h>
#include <rendergroup.h>
#include <iostream>

GraphicsManager* Graphics;

/*void GraphicsManager::push_back(RenderObject* object, int group){
	

}
void GraphicsManager::insert_object_at(RenderObject* object, RenderObject* at){
	int group = at->group;
	if (at == tail[group]){
		tail[group] = object;
		object->previous = NULL;
		object->next = at;
		at->previous = object;
		if (head[group] == NULL){
			head[group] = at;	
		}
		return;
	}
	object->next = at;
	object->previous = at->previous;
	at->previous->next = object;
	at->previous = object;
}
void GraphicsManager::remove_object(RenderObject* object, bool destroy){

}
void GraphicsManager::insert_by_z(RenderObject* object, int group) {//this should only be used for GRAPHICS_GROUP_SHADER_COLOR_SOLID_TRANSPARENT
	object->group = group;
	RenderObject* curr = tail[group];
	while(curr != NULL){
		if (curr->position.z >= object->position.z){
			insert_object_at(object, curr);	
			return;
		}
		curr = curr->next;
	}
	push_back(object); //empty list or object has the biggest z
}

RenderObject* GraphicsManager::new_empty_object(int group){
	RenderObject* object = new RenderObject;
	object->group = group;
	if (group == GRAPHICS_SHADER_COLOR_SOLID_TRANSPARENT){
		insert_by_z(object);
	}
	else {
		push_back(object);
	}
	return object;
}

RenderObject* GraphicsManager::new_copy_object(RenderObject* original){
	RenderObject* copy = new_empty_object(original->group);
	copy->render_data = original->render_data;
	copy->position = original->position;
	copy->position_mat = original->position_mat;
	copy->rotation_mat = original->rotation_mat;
	return copy;
}

RenderObject* GraphicsManager::new_triangles_object(int group, 
													std::size_t size_vertices, float vertices[],
					  								std::size_t size_indices, unsigned int indices[],
													Color color){
	
}*/
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