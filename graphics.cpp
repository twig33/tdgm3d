#include <graphics.h>
#include <iostream>

GraphicsManager* Graphics;

RenderObject::~RenderObject(){
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
	delete this;
}
void RenderObject::set_position(const glm::vec3& pos){
/*	if (pos.z != this->position.z){
		if ((this->previous && this->previous->position.z > pos.z) ||
			(this->next && this->next->position.z < pos.z) ||
		    (!(this->previous) && !(this->next))){
			Graphics->remove_object(this, false);
			Graphics->insert_by_z(this);
		}
	}*/
	position_mat = glm::translate(glm::mat4(1.0f), pos);
	position = pos;
	update_transform_matrix();
}
void RenderObject::set_rotation(const glm::vec3& rot){
	rotation_mat = glm::rotate(glm::mat4(1.0f), glm::radians(rot.x), glm::vec3(1.0, 0.0, 0.0));
	rotation_mat = glm::rotate(rotation_mat, glm::radians(rot.y), glm::vec3(0.0, 1.0, 0.0));
	rotation_mat = glm::rotate(rotation_mat, glm::radians(rot.z), glm::vec3(0.0, 0.0, 1.0));
	update_transform_matrix();
}
void RenderObject::set_scale(const glm::vec3& scalein){
	scale_mat = glm::scale(glm::mat4(1.0f), scalein);
	update_transform_matrix();
}
void RenderObject::update_transform_matrix(){
	transform = scale_mat * position_mat * rotation_mat;
}
void GraphicsManager::remove_object(RenderObject* object, bool destroy){
	int shader_type = object->shader_type;
	if (!object->previous){ //if previous is null then the object is tail
		this->tail[shader_type] = object->next;
		return;
	}
	else if (!object->next){ //if next is null then the object is head
		this->head[shader_type] = object->previous;
		return;
	}
	else {
		object->previous->next = object->next;
		object->next->previous = object->previous;
	}
	if (destroy){
		delete object;
	}
}
void GraphicsManager::insert_by_z(RenderObject* object) {//this should only be used for GRAPHICS_SHADER_COLOR_SOLID_TRANSPARENT
	int shader_type = object->shader_type;
	if (this->tail[shader_type] == NULL){
		this->tail[shader_type] = object;
		return;
	}
	if (this->tail[shader_type]->position.z >= object->position.z){
		RenderObject* old_tail = this->tail[shader_type];
		this->tail[shader_type] = object;
		this->tail[shader_type]->next = old_tail;
		old_tail->previous = this->tail[shader_type];
		return;
	}
	RenderObject* curr = this->tail[shader_type];
	while (curr != NULL && curr->next != NULL){
		if (curr->position.z >= object->position.z){
			RenderObject* right = curr;
			RenderObject* left = curr->previous;
			right->previous = object;
			left->next = object;
			object->previous = left;
			object->next = right;
			return;
		}
		curr = curr->next;
	}
	RenderObject* old_head = this->head[shader_type];
	old_head->next = object;
	object->previous = old_head;
	this->head[shader_type] = object;
}
RenderObject* GraphicsManager::new_empty_object_push_back(int shader_type){
	RenderObject* object = new RenderObject;
	object->shader_type = shader_type;
	if (shader_type == GRAPHICS_SHADER_COLOR_SOLID_TRANSPARENT){
		insert_by_z(object);
		return object;
	}
	if (this->tail[shader_type] == NULL){
		this->tail[shader_type] = object;	
	}else if (this->head[shader_type] == NULL){
		this->head[shader_type] = object;
		this->tail[shader_type]->next = this->head[shader_type];
		this->head[shader_type]->previous = this->tail[shader_type];
	}else{
		this->head[shader_type]->next = object;
		object->previous = this->head[shader_type];
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
	if (shader_type == GRAPHICS_SHADER_COLOR_SOLID && color.a != 1.0){
		std::cout << "Invalid shader type selected, color is transparent, fixing";
		shader_type = GRAPHICS_SHADER_COLOR_SOLID_TRANSPARENT;
	}
	if (shader_type == GRAPHICS_SHADER_COLOR_SOLID_TRANSPARENT && color.a == 1.0){
		std::cout << "Invalid shader type selected, color isn't transparent, fixing";
		shader_type = GRAPHICS_SHADER_COLOR_SOLID;
	}
	RenderObject* object = this->new_empty_object_push_back(shader_type);
	object->vertices_count = size_indices / sizeof(unsigned int);
	if (shader_type == GRAPHICS_SHADER_COLOR_SOLID || shader_type == GRAPHICS_SHADER_COLOR_SOLID_TRANSPARENT){
		object->color = color; 	//this is for the solid color shader only because
	}							//in per vertex color shader colors are in the VBO
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
	shader[shader_type]->bind_attributes();
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
	glEnable(GL_DEPTH_TEST); 
	glEnable(GL_BLEND); 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
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
	shader[GRAPHICS_SHADER_COLOR_SOLID_TRANSPARENT] = shader[GRAPHICS_SHADER_COLOR_SOLID]; //there isnt actually a separate transparency shader
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
		curr = this->tail[i];
		unsigned int old_vao;
		if (curr != NULL){
			old_vao = curr->VAO;
			glBindVertexArray(old_vao);
		}
		while(curr != NULL){
			
			shader[i]->set_transform(glm::value_ptr(curr->transform));
			if (curr->VAO != old_vao){
				glBindVertexArray(curr->VAO);
			}
			do_shader_specific_actions(i, curr);
			glDrawElements(GL_TRIANGLES, curr->vertices_count, GL_UNSIGNED_INT, 0);
			old_vao = curr->VAO;
			curr = curr->next;
		}
	}
	glfwSwapBuffers(window);
	glfwPollEvents();
}