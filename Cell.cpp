#include "Cell.h"

Cell::Cell()
{
	if (fontSet == false)
	{
		if (!font.loadFromFile("PressStart2P-vaV7.ttf"))
		{
			std::cout << "Error could not find font" << std::endl;
		}
		fontSet = true;

	}
	gridSquare.setSize(sf::Vector2f(.5f, .5f));
	gridSquare.setFillColor(sf::Color::Blue);
	gridSquare.setOutlineColor(sf::Color::Black);
	cost = 1;
	type = 0; // passable
}

Cell::Cell(sf::Vector2f worldPosition, sf::Vector2i gridPosition, float cellSize)
{
	this->worldPosition = worldPosition;
	this->gridPosition = gridPosition;
	gridSquare.setSize(sf::Vector2f(cellSize, cellSize));
	gridSquare.setOrigin(cellSize / 2, cellSize / 2);
	gridSquare.setFillColor(sf::Color::Blue);
	gridSquare.setOutlineColor(sf::Color::Black);
	gridSquare.setOutlineThickness(1.0f);
	gridSquare.setPosition(worldPosition);
	cost = 1;
	type = 0; // passable;
	costText.setCharacterSize(8);
	costText.setFont(font);
	costText.setFillColor(sf::Color::White);
	costText.setPosition(worldPosition.x - cellSize/2, worldPosition.y);
}

Cell::~Cell()
{
}

sf::Vector2f Cell::getWorldPosition()
{
	return worldPosition;
}

sf::Vector2i Cell::getGridPosition()
{
	return gridPosition;
}

sf::RectangleShape Cell::getGridSquare()
{
	return gridSquare;
}

sf::Text Cell::getCostText()
{
	return costText;
}

void Cell::increaseCost(int amount)
{
	cost += amount;
	if (cost >= 65000)
	{
		cost = 65000;
		return;
	}

}

void Cell::setCost(int amount)
{
	cost = 0;
	increaseCost(amount);
}

int Cell::getCost()
{
	return cost;
}

int Cell::getBestCost()
{
	return bestCost;
}

void Cell::setBestCost(int bestCost)
{
	this->bestCost = bestCost;
}

int Cell::getType()
{
	return type;
}

void Cell::setType(int type)
{
	this->type = type;

	if (type == PASSABLE)
	{
		bestCost = 1;
		switchCostTextDisplay();
		return;
	}
	if (type == START)
	{
		bestCost = 0;
		switchCostTextDisplay();
		return;
	}
	bestCost = 65000;
	switchCostTextDisplay();
}

void Cell::setBlockedColour()
{
	gridSquare.setFillColor(sf::Color::Black);
}

void Cell::setPassableColour()
{
	sf::Uint8 colourToSet = (sf::Uint8)(255 - bestCost);
	if (colourToSet > 255 || colourToSet < 0)
	{
		colourToSet = 0;
	}
	sf::Color col = sf::Color(0, 0, colourToSet, colourToSet);
	gridSquare.setFillColor(col);
}

void Cell::setStartColour()
{
	gridSquare.setFillColor(sf::Color::Green);
}

void Cell::setEndColour()
{
	gridSquare.setFillColor(sf::Color::Red);
}

void Cell::setPathColour()
{
	gridSquare.setFillColor(sf::Color::Yellow);
}

bool Cell::isStartingCell()
{
	return startingCell;
}

void Cell::setStartingCell(bool startingCell)
{
	this->startingCell = startingCell;
}

void Cell::switchCostTextDisplay()
{
	std::string msg;

	if (Cell::showBest == false)
	{
		msg = std::to_string(cost);
		costText.setString(msg);
		return;
	}

	msg = std::to_string(bestCost);
	costText.setString(msg);
}

void Cell::setShowBest(bool show)
{
	showBest = show;
}

bool Cell::isShowingBestCost()
{
	return showBest;
}

void Cell::setShowCostText(bool show)
{
	showCostText = show;
}

bool Cell::canShowCostText()
{
	return showCostText;
}

void Cell::setBestDirection(sf::Vector2i bestDirection)
{
	this->bestDirection = bestDirection;
}

sf::Vector2i Cell::getBestDirection()
{
	return bestDirection;
}

bool Cell::showBest = true;
bool Cell::showCostText = false;





