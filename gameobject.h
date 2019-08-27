#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <vector>
#include <graphics.h>
#include <collision2d.h>

class GameObject {
	public:
		virtual void update(){std::cout << "Pure virtual function called\n";};
		Transform transform;
		unsigned long id = 0;
	protected:
		//GameObject(); //disallow instantiating GameObject class
		RenderObject* render_object;
};
class Projectile : public GameObject {
	public:
		Projectile(glm::vec3 speed);
		void update();
	private:
		const glm::vec3 speed;
		float vertices[9] = {
			-1.0f, 0.0f, 0.0f,
			 0.0f, 1.0f, 0.0f,
			 1.0f, 0.0f, 0.0f,
		};
		unsigned int indices[3] = {
			0, 1, 2,	
		};
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
			 0.0f, 0.0f, 0.0f, 0.5f, 1.0f, 1.0f,
			 1.0f,-1.0f, 0.0f, 0.5f, 0.5f, 1.0f,
			 0.0f,-2.0f, 0.0f, 0.5f, 0.5f, 1.0f,
		};
		unsigned int player_indices[9] = {
			0, 1, 2,
			3, 4, 5,
			3, 5, 6
		};
};
class GameObjectManager {
	public:
		GameObjectManager();
		void update();
		void push(GameObject* object);
		void remove(GameObject* object);
	private:
		unsigned long index_by_id(unsigned long id);
		void generate_id_index(unsigned long* id, unsigned long* index);
		unsigned long objects_vector_reserve = 128;
		std::vector<GameObject*> objects;
};
extern GameObjectManager* GameObjects;
#endif