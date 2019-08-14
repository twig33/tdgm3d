#include <graphics.h>
#include <iostream>

GraphicsManager* Graphics;

RenderObject* GraphicsManager::new_empty_object_push_back(int shader_type){
	RenderObject* object = new RenderObject;
	if (this->tail[shader_type] == NULL){
		this->tail[shader_type] = object;	
	}else if (this->head[shader_type] == NULL){
		this->head[shader_type] = object;
		this->tail[shader_type]->next = this->head[shader_type];
	}else{
		this->head[shader_type]->next = object;
		this->head[shader_type] = object;
	}
	return object;
}
RenderObject* GraphicsManager::new_triangles_object(int shader_type, 
													std::size_t size_vertices, float vertices[],
					  								std::size_t size_indices, unsigned int indices[],
													Color color){
	if (size_indices % 3){
		std::cout << "Warning: indices size not a multiply of 3\n";	
	}
	RenderObject* object = this->new_empty_object_push_back(shader_type);
	object->vertices_count = size_indices / sizeof(unsigned int);
	if (shader_type == GRAPHICS_SHADER_COLOR_SOLID){
		object->color = color; //this is for the solid color shader only because
	}						   //in per vertex color shader colors are in the VBO
	//generate vbo ebo vao
	glGenBuffers(1, &(object->VBO));
	glGenBuffers(1, &(object->EBO));
	glGenVertexArrays(1, &(object->VAO));
	//bind vao
	glBindVertexArray(object->VAO);
	//fill data
	glBindBuffer(GL_ARRAY_BUFFER, object->VBO);
	glBufferData(GL_ARRAY_BUFFER, size_vertices, vertices, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size_indices, indices, GL_DYNAMIC_DRAW);
	//attributes are shader specific
	switch (shader_type){
		case GRAPHICS_SHADER_COLOR_SOLID:
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
			break;
		case GRAPHICS_SHADER_COLOR_VERTEX:
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);
			break;
	}
	//cleanup unbind all
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	return object;
}
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
	//create shaders
	glm::mat4 identity = glm::mat4(1.0f);
	glm::vec3 offset = glm::vec3(0.0f,0.0f,-2.5f);
	transform = glm::mat4(1.0f);
	transform = glm::translate(transform, offset);
	proj = glm::perspective(glm::radians(90.0f), (float)640/(float)480, 0.1f, 100.0f);
	camera = glm::mat4(1.0f);
	//glm::vec3 cameraPos = glm::vec3(0.0,0.0,-3.0);
	//camera = glm::lookAt(cameraPos,
	//					 glm::vec3(0.0,0.0,0.0),
	//					 glm::vec3(0.0,1.0,0.0));
	shader[GRAPHICS_SHADER_COLOR_SOLID] = new ShaderColorSolid();
	shader[GRAPHICS_SHADER_COLOR_VERTEX] = new ShaderColorVertex();
	//set screen clear color
	glClearColor(0.9, 0.9, 1, 1);
}

void GraphicsManager::finish(){
	this->quit = 1;
	glfwTerminate();	
}

void GraphicsManager::solid_color_shader_actions(RenderObject* object){
	(static_cast<ShaderColorSolid*>(shader[GRAPHICS_SHADER_COLOR_SOLID]))->set_color( object->color.r,
																		object->color.g,
																		object->color.b,
																		object->color.a); //set the color
}
void GraphicsManager::vertex_color_shader_actions(RenderObject* object){
	//there isnt anything to do yet	
}
void GraphicsManager::do_shader_specific_actions(int shader_type, RenderObject* object){
	(this->*shader_specific_actions[shader_type])(object);
}
void GraphicsManager::update(){
	if (glfwWindowShouldClose(window)){
		this->finish();
		return;	
	}
	//clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//current object which we are drawing
	RenderObject* curr = NULL;
	//iterate over shaders
	for (int i = 0; i < GRAPHICS_SHADER_SIZE; ++i){
		shader[i]->use();
		shader[i]->set_view(glm::value_ptr(camera));
		shader[i]->set_projection(glm::value_ptr(proj));
		shader[i]->set_transform(glm::value_ptr(transform));
		curr = this->tail[i];
		while(curr != NULL){
			glBindVertexArray(curr->VAO);
			do_shader_specific_actions(i, curr);
			glDrawElements(GL_TRIANGLES, curr->vertices_count, GL_UNSIGNED_INT, 0);
			curr = curr->next;
		}
	}
	glfwSwapBuffers(window);
	glfwPollEvents();
}