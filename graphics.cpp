#include <graphics.h>
#include <iostream>
#include <shader.h> //load_shaders();

GraphicsManager Graphics;

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
	object->vertices_count = size_indices;
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
void GraphicsManager::init(){
	//glfw init
	glfwInit();
	//window
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	window = glfwCreateWindow(640, 480, "pong", nullptr,
	nullptr);
	glfwMakeContextCurrent(window);
	//viewport
	glViewport(0, 0, 640, 480);
	//glew init 
	glewExperimental = GL_TRUE; //ya uzhe забылdlya chevo eto
	glewInit();
	//create shader programs
	shader_program[GRAPHICS_SHADER_COLOR_SOLID] = load_shaders("shader_color_solid.vs", "shader_color_solid.frag");
	shader_color_solid_ourColor = glGetUniformLocation(shader_program[GRAPHICS_SHADER_COLOR_SOLID], "ourColor");
	shader_program[GRAPHICS_SHADER_COLOR_VERTEX] = load_shaders("shader_color_vertex.vs", "shader_color_vertex.frag");
	//set screen clear color
	glClearColor(0.9, 0.9, 1, 1);
}

void GraphicsManager::finish(){
	this->quit = 1;
	glfwTerminate();	
}

void GraphicsManager::update(){
	if (glfwWindowShouldClose(window)){
		this->finish();
		return;	
	}
	//clear the screen
	glClear(GL_COLOR_BUFFER_BIT);
	RenderObject* curr = NULL;
	//TODO merge the shaders draw (if i == GRAPHICS_SHADER_COLOR_SOLID could be expensive)
	//solid color shader objects draw
	glUseProgram(shader_program[GRAPHICS_SHADER_COLOR_SOLID]);
	curr = this->tail[GRAPHICS_SHADER_COLOR_SOLID];
	while(curr != NULL){
		glBindVertexArray(curr->VAO);
		glUniform4f(shader_color_solid_ourColor, curr->color.r, curr->color.g, curr->color.b, curr->color.a);
		glDrawElements(GL_TRIANGLES, curr->vertices_count, GL_UNSIGNED_INT, 0);
		curr = curr->next;
	}
	//per vertex color shader objects draw
	glUseProgram(shader_program[GRAPHICS_SHADER_COLOR_VERTEX]);
	curr = this->tail[GRAPHICS_SHADER_COLOR_VERTEX];
	while(curr != NULL){
		glBindVertexArray(curr->VAO);
		glDrawElements(GL_TRIANGLES, curr->vertices_count, GL_UNSIGNED_INT, 0);
		curr = curr->next;
	}
	glfwSwapBuffers(window);
	glfwPollEvents();
}