#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <graphics.h>
#include <collision2d.h>

class GameObject {
	public:
		virtual void update(){};
		Transform transform;
	protected:
		//GameObject(); //disallow instantiating GameObject class
		RenderObject* render_object;
};
class Player : public GameObject {
	public:
		Player();
		void update();
	private:
		float player_vertices[42] = {
			-1.0f, 0.0f, 0.0f, 0.5f, 0.5f, 1.0f,
			 0.0f, 1.0f, 0.0f, 0.5f, 1.0f, 1.0f,
			 1.0f, 0.0f, 0.0f, 0.5f, 0.5f, 1.0f,
			-1.0f,-1.0f, 0.0f, 0.5f, 0.5f, 1.0f,
			 0.0f, 0.0f, 0.0f, 0.5f, 0.5f, 1.0f,
			 1.0f,-1.0f, 0.0f, 0.5f, 0.5f, 1.0f,
			 0.0f,-2.0f, 0.0f, 0.5f, 0.5f, 1.0f,
		};
		unsigned int player_indices[9] = {
			0, 1, 2,
			3, 4, 5,
			3, 5, 6
		};
};
#endif