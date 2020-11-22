#include "FlowField.h"

FlowField::FlowField()
{
 
}

FlowField::FlowField(sf::Vector2i gridSize, float cellRadius)
{
	this->gridSize = gridSize;
	this->cellRadius = cellRadius;
	cellDiameter = cellRadius * 2.0f;
}

FlowField::~FlowField()
{
}

std::vector<std::vector<Cell*>> FlowField::getGrid()
{
	return grid;
}

sf::Vector2i FlowField::getGridSize()
{
	return gridSize;
}

float FlowField::getCellRadius()
{
	return cellRadius;
}

sf::RectangleShape FlowField::getGridSquareAt(int x, int y)
{
	return grid[y][x]->getGridSquare();
}

sf::Text FlowField::getCostTextAt(int x, int y)
{
	return grid[y][x]->getCostText();
}

void FlowField::createGrid()
{
	// if grid isn't empty we delete previous pointers before assigning new ones
	if (grid.empty() == false)
	{
		for (int y = 0; y < gridSize.y; y++)
		{
			for (int x = 0; x < gridSize.x; x++)
			{
				delete grid[y][x];
			}
		}
	}
	grid = std::vector<std::vector<Cell*>>();

	grid.reserve(gridSize.y);
	for (int y = 0; y < gridSize.y; y++)
	{
		grid.push_back(std::vector <Cell*>());
		grid[y].reserve(gridSize.x);
		for (int x = 0; x < gridSize.x; x++)
		{
			sf::Vector2f worldPosition = { cellDiameter * x + cellRadius, cellDiameter * y + cellRadius };
			Cell* cell = new Cell(worldPosition, sf::Vector2i(x, y), cellDiameter);
			
			grid[y].push_back(cell);
		}

	}
}

void FlowField::reassignGridValues()
{
	for (int y = 0; y < gridSize.y; y++)
	{
		for (int x = 0; x < gridSize.x; x++)
		{
			Cell* cell = grid[y][x];
			if (cell->getType() == IMPASSABLE)
			{
				cell->setBestCost(9999999);
				cell->setCost(65000);
			}
			else
			{
				cell->setBestCost(9999999);
				cell->setCost(1);
			}
		}
	}
}

void FlowField::setCostField()
{
	for (int y = 0; y < gridSize.y; y++)
	{
		for (int x = 0; x < gridSize.x; x++)
		{
			if (grid[y][x]->getType() == IMPASSABLE)
			{
				grid[y][x]->setCost(65000);
				getGridSquareAt(x, y).setFillColor(sf::Color::Green);
				continue;
			}
			grid[y][x]->setCost(1);
			getGridSquareAt(x, y).setFillColor(sf::Color::Blue);
		}

	}
}

int FlowField::paintType(sf::Vector2i mousePosition)
{
	if (!(mousePosition.x < cellDiameter * gridSize.x))
	{
		std::cout << "outside x bounds" << std::endl;
		return -1;
	}
	if (!(mousePosition.y < cellDiameter * gridSize.y))
	{
		std::cout << "outside y bounds" << std::endl;
		return - 1;
	}
	int x = mousePosition.x / 20;
	int y = mousePosition.y / 20;

	return grid[y][x]->getType();
}

void FlowField::setTileAs(int type, sf::Vector2i mousePosition)
{
	int x = mousePosition.x / 20;
	int y = mousePosition.y / 20;
	if (!(mousePosition.x < cellDiameter * gridSize.x))
	{
		return;
	}
	if (!(mousePosition.y < cellDiameter * gridSize.y))
	{
		return;
	}

	

	if (grid[y][x]->getType() == type)
	{
		return;
	}
	if (type == PASSABLE)
	{
		grid[y][x]->setPassableColour();
	}
	else
	{
		grid[y][x]->setBlockedColour();
	}
	grid[y][x]->setType(type);
	
}

void FlowField::createIntegrationField(Cell *destinationCell)
{
	if (nullptr == destinationCell)
	{
		return;
	}
	std::queue<Cell*> cellsToCheck;

	cellsToCheck.push(destinationCell);

	destinationCell->setCost(0);
	destinationCell->setBestCost(0);

	while (cellsToCheck.size() != 0)
	{
		Cell* currentCell = cellsToCheck.front();
		std::vector<Cell*> currentCellNeighbors = getNeighborCells(currentCell->getGridPosition());
		int numberOfNeighbors = currentCellNeighbors.size();
		for(int index = 0; index < numberOfNeighbors; index++)
		{
			if (currentCellNeighbors[index]->getCost() >= 65000)
			{

				currentCellNeighbors[index]->setBlockedColour();
				continue;
			}
			if (currentCellNeighbors[index]->isStartingCell())
			{
				continue;
			}

			if (currentCellNeighbors[index]->getCost() + currentCell->getBestCost() < currentCellNeighbors[index]->getBestCost())
			{
				currentCellNeighbors[index]->setBestCost(currentCellNeighbors[index]->getCost() + currentCell->getBestCost());
				
				cellsToCheck.push(currentCellNeighbors[index]);
				
			}
			if (currentCellNeighbors[index] == destinationCell)
			{
				continue;
			}
			
			currentCellNeighbors[index]->setPassableColour();
		}
		cellsToCheck.pop();
	}


}

// Cardinal Direction neighbours
std::vector<Cell*> FlowField::getNeighborCells(sf::Vector2i gridIndex)
{
	std::vector<Cell*> myNeighbors = std::vector<Cell*>();

	if (gridIndex.x > 0)
	{
		myNeighbors.push_back(grid[gridIndex.y][gridIndex.x - 1]);
	}

	if (gridIndex.x < gridSize.x - 1)
	{
		myNeighbors.push_back(grid[gridIndex.y][gridIndex.x + 1]);
	}
	if (gridIndex.y > 0)
	{
		myNeighbors.push_back(grid[gridIndex.y - 1][gridIndex.x]);
		if (gridIndex.x > 0)
		{
			myNeighbors.push_back(grid[gridIndex.y - 1][gridIndex.x - 1]);
		}

		if (gridIndex.x < gridSize.x - 1)
		{
			myNeighbors.push_back(grid[gridIndex.y - 1][gridIndex.x + 1]);
		}
	}

	if (gridIndex.y < gridSize.y - 1)
	{
		myNeighbors.push_back(grid[gridIndex.y + 1][gridIndex.x]); 
		
		if (gridIndex.x > 0)
		{
			myNeighbors.push_back(grid[gridIndex.y + 1][gridIndex.x - 1]);
		}

		if (gridIndex.x < gridSize.x - 1)
		{
			myNeighbors.push_back(grid[gridIndex.y + 1][gridIndex.x + 1]);
		}
	}

	return myNeighbors;
}

Cell* FlowField::getCellAtMousePosition(sf::Vector2i mousePosition)
{
	int x = mousePosition.x / 20;
	int y = mousePosition.y / 20;

	if (x >= gridSize.x || x < 0)
	{
		return nullptr;
	}

	if (y >= gridSize.y || y < 0)
	{
		return nullptr;
	}

	return grid[y][x]; 
}

void FlowField::createFlowField()
{
	for (int y = 0; y < gridSize.y; y++)
	{
		for (int x = 0; x < gridSize.x; x++)
		{
			Cell* current = grid[y][x];
			std::vector<Cell*> currentCellNeighbors = getNeighborCells(current->getGridPosition());

			int bestCost = current->getBestCost();

			int noOfNeighbors = currentCellNeighbors.size();

			for (int index = 0; index < noOfNeighbors; index++)
			{
				if (currentCellNeighbors[index]->getBestCost() < bestCost)
				{
					bestCost = currentCellNeighbors[index]->getBestCost();
					current->setBestDirection(currentCellNeighbors[index]->getGridPosition() - current->getGridPosition());
				}
			}
		}
	}
}

void FlowField::showBestPath(Cell* start, Cell* end)
{
	if (start == nullptr)
	{
		return;
	}
	if (end == nullptr)
	{
		return;
	}
	//Make sure end is not the start
	if (end->isStartingCell())
	{
		return;
	}
	if (end->getType() == IMPASSABLE)
	{
		return;
	}
	if (start->getType() == IMPASSABLE)
	{
		return;
	}
	
	Cell* current = start;

	// prevent infinite loop
	int maxChecks = gridSize.x * gridSize.y;
	int checksRun = 0;
	while (current != end && checksRun < maxChecks)
	{
		sf::Vector2i bestDirection = current->getBestDirection() + current->getGridPosition();
		current = grid[bestDirection.y][bestDirection.x];

		if (current == end)
		{
			break;
		}
		checksRun++;
		current->setPathColour();
	}
}


