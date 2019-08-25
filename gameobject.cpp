#include <gameobject.h>
#include <graphics.h>
#include <input.h>

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
}