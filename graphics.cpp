#include <graphics.h>
#include <iostream>

GraphicsManager* Graphics;

RenderObject::~RenderObject(){
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
	//delete this;
}
void RenderObject::set_color(const float r, const float g, const float b, const float a){
	color = {r, g, b, a};	
}
void RenderObject::set_position(const glm::vec3& pos){
	float old_position_z = position.z;
	position_mat = glm::translate(glm::mat4(1.0f), pos);
	position = pos;
	update_transform_matrix();
	if (this->shader_type == GRAPHICS_SHADER_COLOR_SOLID_TRANSPARENT && pos.z != old_position_z){
		Graphics->remove_object(this, false);
		Graphics->insert_by_z(this);
	}
}
void RenderObject::translate(const glm::vec3& tr){
	position_mat = glm::translate(position_mat, tr);
	position += tr;
	update_transform_matrix();
	if (tr.z != 0 && shader_type == GRAPHICS_SHADER_COLOR_SOLID_TRANSPARENT){
		Graphics->remove_object(this, false);
		Graphics->insert_by_z(this);
	}
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

void GraphicsManager::push_back(RenderObject* object){
	int shader_type = object->shader_type;
	if (tail[shader_type] == NULL){
		tail[shader_type] = object;
		return;
	}
	if (head[shader_type] == NULL){
		head[shader_type] = object;
		object->previous = tail[shader_type];
		object->next = NULL;
		tail[shader_type]->next = object;
		return;
	}
	head[shader_type]->next = object;
	object->previous = head[shader_type];
	object->next = NULL;
	head[shader_type] = object;
	return;
}
void GraphicsManager::insert_object_at(RenderObject* object, RenderObject* at){
	int shader_type = at->shader_type;
	if (at == tail[shader_type]){
		tail[shader_type] = object;
		object->previous = NULL;
		object->next = at;
		at->previous = object;
		if (head[shader_type] == NULL){
			head[shader_type] = at;	
		}
		return;
	}
	object->next = at;
	object->previous = at->previous;
	at->previous->next = object;
	at->previous = object;
}
void GraphicsManager::remove_object(RenderObject* object, bool destroy){
	int shader_type = object->shader_type;
	if (object == tail[shader_type]){
		tail[shader_type] = NULL;
		if (object->next != NULL){
			tail[shader_type] = object->next;
			tail[shader_type]->previous = NULL;
		}
	}
	else if (object == head[shader_type]){
		if (object->previous == tail[shader_type]){
			head[shader_type] = NULL;
			object->previous->next = NULL;
		}
		else {
			head[shader_type] = object->previous;
			head[shader_type]->next = NULL;	
		}
	}
	else {
		object->previous->next = object->next;
		object->next->previous = object->previous;
	}
	object->previous = NULL;
	object->next = NULL;
	if (destroy){
		delete object;
	}
}
void GraphicsManager::insert_by_z(RenderObject* object) {//this should only be used for GRAPHICS_SHADER_COLOR_SOLID_TRANSPARENT
	int shader_type = object->shader_type;
	RenderObject* curr = tail[shader_type];
	while(curr != NULL){
		if (curr->position.z >= object->position.z){
			insert_object_at(object, curr);	
			return;
		}
		curr = curr->next;
	}
	push_back(object); //empty list or object has the biggest z
}

RenderObject* GraphicsManager::new_empty_object(int shader_type){
	RenderObject* object = new RenderObject;
	object->shader_type = shader_type;
	if (shader_type == GRAPHICS_SHADER_COLOR_SOLID_TRANSPARENT){
		insert_by_z(object);
	}
	else {
		push_back(object);
	}
	return object;
}

RenderObject* GraphicsManager::new_copy_object(RenderObject* original){
	RenderObject* copy = new_empty_object(original->shader_type);
	copy->shader_type = original->shader_type;
	copy->state = original->state;
	copy->color = original->color;
	copy->VAO = original->VAO;
	copy->VBO = original->VBO;
	copy->EBO = original->EBO;
	copy->vertices_count = original->vertices_count;
	copy->bounds_left = original->bounds_left;
	copy->bounds_right = original->bounds_right;
	copy->bounds_up = original->bounds_up;
	copy->bounds_down = original->bounds_down;
	return copy;
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
	RenderObject* object = this->new_empty_object(shader_type);
	object->vertices_count = size_indices / sizeof(unsigned int);
	if (shader_type == GRAPHICS_SHADER_COLOR_SOLID || shader_type == GRAPHICS_SHADER_COLOR_SOLID_TRANSPARENT){
		object->color = color; 	//this is for the solid color shader only because
	}							//in per vertex color shader colors are in the VBO
	object->bounds_left = vertices[0];
	object->bounds_right = vertices[0];
	object->bounds_up = vertices[1];
	object->bounds_down = vertices[1];
	for (int i = 0; i < object->vertices_count * shader[shader_type]->stride; i += shader[shader_type]->stride){
		if (object->bounds_left > vertices[i]){
			object->bounds_left = vertices[i];	
		}
		if (object->bounds_right < vertices[i]){
			object->bounds_right = vertices[i];	
		}
		if (object->bounds_up < vertices[i+1]){
			object->bounds_up = vertices[i+1];	
		}
		if (object->bounds_down > vertices[i+1]){
			object->bounds_down = vertices[i+1];	
		}
	}
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