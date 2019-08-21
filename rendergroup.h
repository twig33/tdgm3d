#ifndef RENDERGROUP_H
#define RENDERGROUP_H

#include <shader.h>
#include <renderobject.h>

class RenderGroup {
	friend class RenderGroupManager;
	public:
		virtual void update(){};
		virtual void push_node(RenderObjectNode* node);
		virtual void remove_node(RenderObjectNode* node, bool destroy = true);
		RenderObjectNode* find(RenderObject* object);
	//	void insert_object_at(RenderObject* object, RenderObject* at);
		const unsigned int group_type;
		const unsigned int shader_type;
		RenderObjectNode* tail = NULL; //doubly linked list
		RenderObjectNode* head = NULL; //doubly linked list
	private:
		RenderGroup(unsigned int group_type, unsigned int shader_type) : group_type(group_type), shader_type(shader_type) {};
};

class OldPosZData {
	friend class RenderGroupTransparent;
	private:
		~OldPosZData(){};
		float old_pos_z = 0;
};
class RenderGroupTransparent : public RenderGroup {
	public:
		void update();
		void push_node(RenderObjectNode* node);
		void remove_node(RenderObjectNode* node, bool destroy = true);
	private:
		using RenderGroup::RenderGroup;
		void insert_by_z(RenderObjectNode* node);
		
};
class RenderGroupManager {
	public:
		//no constructor needed right now
		void draw();
		void push(RenderObject* object);
		void remove(RenderObject* object, bool destroy = true);
		void set_projection(glm::mat4& proj_in);
		void set_view(glm::mat4& view);
	private:
		glm::mat4 proj = glm::mat4(1.0f);
		glm::mat4 camera = glm::mat4(1.0f);
		unsigned int determine_group(RenderObject* object);
		void check_group_members(unsigned int group);
		void draw_group(unsigned int group);
		void draw_groups();
		void update_groups();
		
		typedef void (RenderGroupManager::*void_func)(const RenderObject*); //shader specific action function pointer typedef
		void solid_color_shader_actions(const RenderObject* object); 
		void vertex_color_shader_actions(const RenderObject* object);
		void do_shader_specific_actions(unsigned int shader_type, const RenderObject* object); //wrapper because using function pointers is ugly
		void_func shader_specific_actions[GRAPHICS_SHADER_SIZE] = { //shader specific action function pointers storage
			shader_specific_actions[GRAPHICS_SHADER_COLOR_SOLID] = 	&RenderGroupManager::solid_color_shader_actions,
			shader_specific_actions[GRAPHICS_SHADER_COLOR_VERTEX] = &RenderGroupManager::vertex_color_shader_actions,
		};
		enum {
			GROUP_SHADER_COLOR_VERTEX,
			GROUP_SHADER_COLOR_SOLID,
			GROUP_SHADER_COLOR_SOLID_TRANSPARENT, //right now there can only be one transparent group
			GROUP_SIZE
		};
		RenderGroup* groups[GROUP_SIZE] = {
			groups[GROUP_SHADER_COLOR_VERTEX] = new RenderGroup(GROUP_SHADER_COLOR_SOLID, GRAPHICS_SHADER_COLOR_VERTEX),
			groups[GROUP_SHADER_COLOR_SOLID] = new RenderGroup(GROUP_SHADER_COLOR_SOLID, GRAPHICS_SHADER_COLOR_SOLID),
			groups[GROUP_SHADER_COLOR_SOLID_TRANSPARENT] = new RenderGroupTransparent(GROUP_SHADER_COLOR_SOLID_TRANSPARENT, GRAPHICS_SHADER_COLOR_SOLID),
		};
};	
extern RenderGroupManager* RenderGroups;
#endif