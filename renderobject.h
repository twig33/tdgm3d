#ifndef RENDEROBJECT_H
#define RENDEROBJECT_H

#include <transform.h>
struct Color {
	float r = 0;
	float g = 0;
	float b = 0;
	float a = 1;
};

class VertexData {
	friend class RenderObject;
	friend class GraphicsManager;
	friend class RenderGroupManager;
	public:
		VertexData(unsigned int st, std::size_t size_vertices, float vertices[],
									std::size_t size_indices, unsigned int indices[]);
		const unsigned int shader_type;
		float get_bounds_left(){
			return bounds_left;	
		}
		float get_bounds_right(){
			return bounds_right;	
		}
		float get_bounds_up(){
			return bounds_up;	
		}
		float get_bounds_down(){
			return bounds_down;	
		}
		unsigned int get_vertices_count(){
			return vertices_count;	
		}
	//	unsigned int get_VAO({
	//		return VAO;	
	//	}
	private:
		~VertexData();
		float bounds_left;
		float bounds_right;
		float bounds_up;
		float bounds_down;
		unsigned int vertices_count;
		unsigned int elements_count;
		unsigned int VBO;
		unsigned int EBO;
		unsigned int VAO;
};

class RenderObject {
	friend class GraphicsManager;
	friend class RenderGroupManager;
	public:
		RenderObject (VertexData* vertex_data, Transform* transform = NULL) : 	vertex_data(vertex_data), 
																				transform(transform ? transform : new Transform()) {};
		Transform* const transform;															/*check if transform is null*/
		VertexData* const vertex_data;
		void set_cycle_colors(bool in);
		bool get_cycle_colors();
		void set_color(float r, float g, float b, float a = -1.0f);
		Color get_color();
	private:
		//~RenderObject();
		bool cycle_colors = false;
		Color color = {1.0f, 1.0f, 1.0f, 1.0f};
};
class RenderObjectNode {
	public:
		RenderObjectNode(RenderObject* object) : object(object) {};
		RenderObject* const object;
		void* group_data = NULL;
		RenderObjectNode* next = NULL;
		RenderObjectNode* previous = NULL;
};
#endif