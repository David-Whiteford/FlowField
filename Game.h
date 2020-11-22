#pragma once
#include <SFML/Graphics.hpp>
#include "FlowField.h"

class Game
{
public:
	Game();
	~Game();
	void run();
	void render();
	void update(float deltaTime);
	void input();

private:
	void updateGrid();

	bool mouseButtonDown = false;
	bool openPath = false;
	bool blockPath = false;
	sf::RenderWindow window;
	bool exitGame = false;
	float deltaTime = 0;
	FlowField flowField;
	Cell* start;
	Cell* end;

};