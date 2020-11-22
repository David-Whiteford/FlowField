#pragma once
#include "Cell.h"
#include <iostream>
#include <queue>

class FlowField
{
public:
	FlowField();
	FlowField(sf::Vector2i gridSize, float cellRadius);
	~FlowField();

	std::vector<std::vector<Cell*>> getGrid();
	sf::Vector2i getGridSize();
	float getCellRadius();
	sf::RectangleShape getGridSquareAt(int x, int y);
	sf::Text getCostTextAt(int x, int y);
	void createGrid();
	void reassignGridValues();
	void setCostField();
	int paintType(sf::Vector2i mousePosition);

	void setTileAs(int type, sf::Vector2i mousePosition);
	void createIntegrationField(Cell *destinationCell);

	std::vector<Cell*> getNeighborCells(sf::Vector2i gridIndex);

	Cell* getCellAtMousePosition(sf::Vector2i mousePosition);
	void createFlowField();
	void showBestPath(Cell*, Cell*);
private:
	std::vector<std::vector<Cell*>> grid;
	sf::Vector2i gridSize;
	float cellRadius;
	float cellDiameter;
	Cell destinationCell;
};