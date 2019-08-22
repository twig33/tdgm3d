#include <random>
#include <graphics.h>
#include <gamestate.h>

void Game::init(){
	triangle = new VertexData( GRAPHICS_SHADER_COLOR_SOLID, sizeof(vertices), vertices,
									   						sizeof(indices), indices);
}

void Game::update(){
	++clock;
	if (clock > 5){
		clock = 0;	
	}
	if (clock == 0){
		++clock_even;
		if (clock_even > 1){
			clock_even = 0;	
		}
		RenderObject* temp = new RenderObject(triangle);
		Graphics->push(temp);
		position.x += speed.x;
		position.y += speed.y;
		if (position.x > 3.0f || position.x < -3.0f){
			speed.x *= -1;	
		}
		if (position.y > 3.0f || position.y < -3.0f){
			speed.y *= -1;	
		}
		if (clock_even%2){
			temp->set_cycle_colors(true);	
			temp->transform.set_position(glm::vec3(position.x, position.y, -4.1f));
		}else {
			temp->set_color(0.0, 1.0, 0.0, 0.5);
			temp->transform.set_position(glm::vec3(position.x, position.y, -4.0f));
		}
	}
}

void Game::finish(){
	
}