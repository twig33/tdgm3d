#ifndef GAMESTATE_H
#define GAMESTATE_H

class GameState {
	public:
		virtual void init(){};
		virtual void update(){};
		virtual void finish(){};
};
class Game : public GameState {
	public:
		void init();
		void update();
		void finish();
	private:
		VertexData* triangle;
		glm::vec2 position = glm::vec2(-3.0f, -3.0f);
		glm::vec2 speed = glm::vec2(0.02f, 0.05f);
		float vertices[9] = {
		   -1.0,-0.9, 0.0,
			0.0, 0.0, 0.0,
			1.0,-0.9, 0.0,
		};
		unsigned int indices[3] = {
			0, 1, 2
		};
		int clock = 0;
		int clock_even = 0;
};
#endif