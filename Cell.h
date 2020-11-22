#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

const int PASSABLE = 0;
const int IMPASSABLE = 1;
const int START = 2;
static sf::Font font;
static bool fontSet = false;

class Cell
{
public:
	Cell();
	Cell(sf::Vector2f worldPosition, sf::Vector2i gridPosition, float cellSize);
	~Cell();

	sf::Vector2f getWorldPosition();
	sf::Vector2i getGridPosition();

	sf::RectangleShape getGridSquare();
	sf::Text getCostText();
	
	void increaseCost(int amount);
	void setCost(int amount);
	int getCost();
	int getBestCost();

	void setBestCost(int bestCost);

	int getType();
	void setType(int type);

	void setBlockedColour();
	void setPassableColour();
	void setStartColour();
	void setEndColour();
	void setPathColour();

	bool isStartingCell();
	void setStartingCell(bool startingCell);

	void switchCostTextDisplay();
	static void setShowBest(bool show);
	static bool isShowingBestCost();
	static void setShowCostText(bool show);
	static bool canShowCostText();

	void setBestDirection(sf::Vector2i bestDirection);
	sf::Vector2i getBestDirection();
private:
	sf::Vector2f worldPosition;
	sf::Vector2i gridPosition;

	sf::RectangleShape gridSquare;

	sf::Text costText;

	int cost;
	int type;
	int bestCost = 999999;
	bool startingCell = false;

	sf::Vector2i bestDirection;

	static bool showBest;
	static bool showCostText;
};
