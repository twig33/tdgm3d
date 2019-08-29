#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <renderobject.h>
#include <rendergroup.h>

RenderGroupManager* RenderGroups;

// -------
//internal linked list functions
void push_back_node(RenderObjectNode** tail, RenderObjectNode** head, RenderObjectNode* node){
	//std::cout << "Pushing back node with pos z " << node->object->transform->get_position().z << " \n";
	if (*tail == NULL){
		*tail = node;
		return;
	}
	if (*head == NULL){
		*head = node;
		node->previous = *tail;
		node->next = NULL;
		(*tail)->next = node;
		return;
	}
	(*head)->next = node;
	node->previous = *head;
	node->next = NULL;
	(*head) = node;
	return;	
}
RenderObjectNode* find(RenderObjectNode** tail, RenderObjectNode** head, RenderObject* object){
	RenderObjectNode* curr = *tail;
	while (curr != NULL){
		if (curr->object == object){
			return curr;	
		}
		curr = curr->next;
	}
	return NULL;
}
void remove_node(RenderObjectNode** tail, RenderObjectNode** head, RenderObjectNode* node){
	//std::cout << "Removing node with " << node->object->transform->get_position().z << " z\n";
	if (node == *tail){
		*tail = NULL;
		if (node->next != NULL){
			*tail = node->next;
			(*tail)->previous = NULL;
		}
	}
	else if (node == *head){
		if (node->previous == *tail){
			*head = NULL;
			node->previous->next = NULL;
		}
		else {
			*head = node->previous;
			node->previous->next = NULL;
			node->next = NULL;	
		}
	}
	else {
		node->previous->next = node->next;
		node->next->previous = node->previous;
	}
	node->previous = NULL;
	node->next = NULL;
}
void insert_node_at(RenderObjectNode** tail, RenderObjectNode** head, RenderObjectNode* node, RenderObjectNode* at){
	if (at == *tail){
		*tail = node;
		node->previous = NULL;
		node->next = at;
		at->previous = node;
		if (*head == NULL){
			*head = at;	
		}
		return;
	}
	node->next = at;
	node->previous = at->previous;
	at->previous->next = node;
	at->previous = node;	
}
//internal linked list functions
// -------
RenderObjectNode* RenderGroup::find(RenderObject* object){
	return ::find(&tail, &head, object);
}

void RenderGroup::push_node(RenderObjectNode* node){
	//std::cout << "Pushing node with pos z " << node->object->transform->get_position().z << " to group " << group_type << "\n";
	RenderObjectNode* curr = tail;
	while (curr != NULL){
		if (curr->object->vertex_data == node->object->vertex_data){
			::insert_node_at(&tail, &head, node, curr);
			return;
		}
		curr = curr->next;
	}
	::push_back_node(&tail, &head, node);
}

void RenderGroup::remove_node(RenderObjectNode* node, bool destroy){
	::remove_node(&tail, &head, node);
	if (destroy){
		delete node;	
	}
}

void RenderGroupSolidColorCycle::update(){
	float r = 0.0f, g = 0.0f, b = 0.0f;
	color += speed;
	if (color >= 0 && color <= 1){
		r = 1 - color;
		g = color;
	}
	else if (color > 1 && color <= 2){
		g = 1 - (color - 1);
		b = color - 1;
	}
	else if (color > 2 && color <= 3){
		b = 1 - (color - 2);
		r = color - 2;
	}
	else {
		color = 0;	
		r = 1 - color;
		g = color;
	}
	RenderObjectNode* curr = tail;
	while (curr != NULL){
		curr->object->set_color(r, g, b, 1.0f);
		curr = curr->next;
	}
}
void RenderGroupTransparent::insert_by_z(RenderObjectNode* node){
	RenderObjectNode* curr = tail;
	while(curr != NULL){
		if ((curr->object)->transform->get_position().z >= node->object->transform->get_position().z){
			::insert_node_at(&tail, &head, node, curr);	
			return;
		}
		curr = curr->next;
	}
	::push_back_node(&tail, &head, node); //empty list or object has the biggest z
}

void RenderGroupTransparent::push_node(RenderObjectNode* node){
	node->group_data = new OldPosZData();
	static_cast<OldPosZData*>(node->group_data)->old_pos_z = node->object->transform->get_position().z;
	insert_by_z(node);	
}

void RenderGroupTransparent::remove_node(RenderObjectNode* node, bool destroy){
	::remove_node(&tail, &head, node);
	delete static_cast<OldPosZData*>(node->group_data); //no longer belongs to this group, must delete group specific data
	if (destroy){
		delete node;
	}
}

void RenderGroupTransparent::update(){
	RenderObjectNode* curr = tail;
	RenderObjectNode* temp_tail = NULL;
	RenderObjectNode* temp_head = NULL;
	while(curr != NULL){
		if (static_cast<OldPosZData*>(curr->group_data)->old_pos_z != curr->object->transform->get_position().z){
			RenderObjectNode* old_curr = curr;
			curr = curr->next;
			::remove_node(&tail, &head, old_curr);
			::push_back_node(&temp_tail, &temp_head, old_curr);
			continue;
		}
		curr = curr->next;
	}
	curr = temp_tail;
	while(curr != NULL){
		::remove_node(&temp_tail, &temp_head, curr);
		push_node(curr); //inserts into correct place by z
		curr = temp_tail; //temp list will keep shrinking until the tail is null
	}
	curr = tail;
	while (curr != NULL){
		static_cast<OldPosZData*>(curr->group_data)->old_pos_z = curr->object->transform->get_position().z;
		curr = curr->next;
	}
}

unsigned int RenderGroupManager::determine_group(RenderObject* object){
	switch(object->vertex_data->shader_type){
		case GRAPHICS_SHADER_COLOR_VERTEX:
			return GROUP_SHADER_COLOR_VERTEX;
			break;
		case GRAPHICS_SHADER_COLOR_SOLID:
			if (object->get_color().a < 1.0){
				return GROUP_SHADER_COLOR_SOLID_TRANSPARENT;	
			}
			else{
				if (object->get_cycle_colors()){
					return GROUP_SHADER_COLOR_SOLID_CYCLE;	
				}else{
					return GROUP_SHADER_COLOR_SOLID;	
				}
			}
			break;
	}
}

void RenderGroupManager::check_group_members(unsigned int group){
	RenderObjectNode* curr = groups[group]->tail;
	while (curr != NULL){
		RenderObjectNode* old_curr = curr;    //need to do this because the node will probably get displaced and next will be changed
		curr = curr->next;
		unsigned int group_determined = determine_group(old_curr->object);
		if (group_determined != group){
			groups[group]->remove_node(old_curr, false);
			groups[group_determined]->push_node(old_curr);
		}
	}
}
void RenderGroupManager::solid_color_shader_actions(const RenderObject* object){
	(static_cast<ShaderColorSolid*>(Shaders->get_shader(GRAPHICS_SHADER_COLOR_SOLID)))->set_color( object->color.r,
																		object->color.g,
																		object->color.b,
																		object->color.a); //set the color
}
void RenderGroupManager::vertex_color_shader_actions(const RenderObject* object){
	//there isnt anything to do yet
}
void RenderGroupManager::do_shader_specific_actions(unsigned int shader_type, const RenderObject* object){
	(this->*shader_specific_actions[shader_type])(object);
}
void RenderGroupManager::draw_group(unsigned int group){
	Shader* shader = Shaders->get_shader(groups[group]->shader_type);
	shader->use();
	shader->set_view(glm::value_ptr(*camera));
	shader->set_projection(glm::value_ptr(*proj));
	RenderObjectNode* curr = groups[group]->tail;
	unsigned int old_vao;
	if (curr != NULL){
		old_vao = curr->object->vertex_data->VAO;
		glBindVertexArray(old_vao);
	}
	while(curr != NULL){
		shader->set_transform((curr->object->transform)->get_transform_mat_value_ptr());
		if (curr->object->vertex_data->VAO != old_vao){
			glBindVertexArray(curr->object->vertex_data->VAO);
		}
		do_shader_specific_actions(curr->object->vertex_data->shader_type, curr->object);
		glDrawElements(GL_TRIANGLES, curr->object->vertex_data->elements_count * 3, GL_UNSIGNED_INT, 0);
		old_vao = curr->object->vertex_data->VAO;
		curr = curr->next;
	}
}
void RenderGroupManager::draw_groups(){
	for (int i = 0; i < GROUP_SIZE; ++i){
		draw_group(i);	
	}
}

void RenderGroupManager::update_groups(){
	for (int i = 0; i < GROUP_SIZE; ++i){
		groups[i]->update();	
	}
}

void RenderGroupManager::draw(){
	check_group_members(GROUP_SHADER_COLOR_SOLID);
	check_group_members(GROUP_SHADER_COLOR_SOLID_CYCLE);
	check_group_members(GROUP_SHADER_COLOR_SOLID_TRANSPARENT); //we only check these right now because 1 group for vertex shader
	update_groups();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	draw_groups();
}
void RenderGroupManager::push(RenderObject* object){
	for (int i = 0; i < GROUP_SIZE; ++i){
		if (groups[i]->find(object)){
			std::cout << "Cannot push renderobject, was pushed already\n";
			return;
		}
	}
	groups[determine_group(object)]->push_node(new RenderObjectNode(object));	
}
void RenderGroupManager::remove(RenderObject* object, bool destroy){
	RenderObjectNode* curr = NULL;
	for (int i = 0; i < GROUP_SIZE; ++i){
		curr = groups[i]->find(object);
		if (curr){
			if (destroy){
				delete curr->object;	
			}
			groups[i]->remove_node(curr, destroy);
			return;
		}
	}
	std::cout << "Couldn't find object node to remove\n";
}
void RenderGroupManager::set_projection_mat_pointer(const glm::mat4 *proj_in){
	proj = proj_in;	
}
void RenderGroupManager::set_view_mat_pointer(const glm::mat4* view){
	camera = view;	
}
