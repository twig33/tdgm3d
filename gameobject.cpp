#include <gameobject.h>
#include <graphics.h>
#include <input.h>

GameObjectManager* GameObjects;
VertexData* temp_proj = NULL;
GameObject::GameObject(){
	GameObjects->push(this);	
}
GameObject::~GameObject(){
	Graphics->remove(render_object);
	delete render_object;
}
Projectile::Projectile(glm::vec3 speed) : speed(speed){
	render_object = new RenderObject(Graphics->get_vertex_data(GRAPHICS_RESOURCE_SPHERE), &transform);
	Graphics->push(render_object);
	//render_object->set_color(0.5f,0.5f,1.0f,1.0f);
	render_object->set_cycle_colors(true);
}
void Projectile::update(){
	transform.translate(speed);	
	//transform.rotate(speed);
	glm::vec3 pos = transform.get_position();
	if (pos.x < -10.0f || pos.x > 10.0f){
		speed.x *= -1;
		GameObjects->remove(this);
		delete this;
		return;
	}
	if (pos.y < -10.0f || pos.y > 10.0f){
		speed.y *= -1;	
		GameObjects->remove(this);
		delete this;
		return;
	}
}
Player::Player(){
	render_object = new RenderObject(new VertexData(GRAPHICS_SHADER_COLOR_VERTEX,
													sizeof(player_vertices), player_vertices,
													sizeof(player_indices), player_indices),
									&transform);
	Graphics->push(render_object);
	render_object->set_color(0.5f,0.5f,1.0f,1.0f);						
}
void Player::update(){
	int axis_x, axis_y;
	if (Input.keys[GLFW_KEY_D] == Input.keys[GLFW_KEY_A]){
		axis_x = 0;	
	}
	else {
		axis_x = Input.keys[GLFW_KEY_D] ? 1 : -1;	
	}
	if (Input.keys[GLFW_KEY_W] == Input.keys[GLFW_KEY_S]){
		axis_y = 0;	
	}
	else {
		axis_y = Input.keys[GLFW_KEY_W] ? 1 : -1;	
	}
	transform.translate(glm::vec3(axis_x * 0.05f, axis_y * 0.05f, 0.0f));
	if (!r_unpressed && !Input.keys[GLFW_KEY_R]){
		r_unpressed = true;	
	}
	if (r_unpressed && Input.keys[GLFW_KEY_R]){
		Projectile* proj = new Projectile(glm::vec3(0.05f, 0.05f, 0.0f));
		proj->transform.set_position(transform.get_position());
		GameObjects->push(proj);
		proj = new Projectile(glm::vec3(-0.05f, -0.05f, 0.0f));
		proj->transform.set_position(transform.get_position());
		GameObjects->push(proj);	
		proj = new Projectile(glm::vec3(-0.05f, 0.05f, 0.0f));
		proj->transform.set_position(transform.get_position());
		GameObjects->push(proj);	
		proj = new Projectile(glm::vec3(0.05f, -0.05f, 0.0f));
		proj->transform.set_position(transform.get_position());
		GameObjects->push(proj);
		r_unpressed = false;
	}
}
GameObjectManager::GameObjectManager(){
	objects.reserve(objects_vector_reserve);	
}

void GameObjectManager::generate_id_index(unsigned long* id, unsigned long* index){
	std::cout << "\n" << objects.size() << "\n";
	for (unsigned long i = 1; i < objects.size(); ++i){
		if (objects[i-1]->id + 1 < objects[i]->id){ //if there is a free space
			*id = objects[i-1]->id + 1;
			*index = i;
			return;
		}
	}
	*id = objects.size() + 1;
	*index = objects.size();
	std::cout << "Generated id " << *id << " index " << *index << "\n";
}

unsigned long GameObjectManager::index_by_id(unsigned long id){
	if (id == 0){
		std::cout << "index_by_id Invalid id: 0\n";
		return objects.size();
	}
	unsigned long r = objects.size() - 1, l = 0, m;
	while (r >= l){
		m = (l + r) / 2;
		if (objects[m]->id == id){
			return m;	
		}
		else if (objects[m]->id > id){
			r = m - 1;
		}
		else if (objects[m]->id < id){
			l = m + 1;	
		}
	}
	std::cout << "Couldn't find index by id\n";
	return objects.size() - 1;
}

void GameObjectManager::update(){
	for (unsigned int i = 0; i < objects.size(); ++i){
		objects[i]->update();	
	}
}
void GameObjectManager::push(GameObject* object){
	if (object->id != 0){
		std::cout << "GameObject was already pushed\n";	
		return;
	}
	if (objects.size() + 1 > objects_vector_reserve){
		std::cout << "Over " << objects_vector_reserve << "game objects\n";
		objects_vector_reserve *= 2;
		objects.reserve(objects_vector_reserve);
	}
	unsigned long id, index;
	generate_id_index(&id, &index);
	object->id = id;
	objects.insert(objects.begin() + index, object);
}
void GameObjectManager::remove(GameObject* object){
	if (object->id == 0){
		std::cout << "GameObjectManager::remove: object wasnt pushed yet\n";	
	}
	objects.erase(objects.begin() + index_by_id(object->id));
}