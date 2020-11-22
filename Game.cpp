#include "Game.h"
#include <iostream>



Game::Game() : window{ sf::VideoMode(1920, 1080, 32), "AI Labs", sf::Style::Fullscreen }
{
	sf::Vector2i gridSize = { 50, 50 };
	flowField = FlowField(gridSize, 10);
	flowField.createGrid();
	flowField.setCostField();
}

Game::~Game()
{
}

void Game::run()
{
	sf::Clock clock;
	sf::Time timeAtEndOfPreviousFrame = sf::Time::Zero;
	sf::Time timePerFrame = sf::seconds((1.f / 120.0f));


	while (window.isOpen() && !exitGame)
	{
		deltaTime = clock.getElapsedTime().asSeconds() - timeAtEndOfPreviousFrame.asSeconds();

		if (clock.getElapsedTime() < timeAtEndOfPreviousFrame + timePerFrame)
		{
			render();
			continue;
		}

		update(deltaTime);
		timeAtEndOfPreviousFrame = clock.getElapsedTime();
		render();
	}
}

void Game::render()
{
	window.clear();

	// don't get vector size of vector as this is slow operation 
	// instead use a vector int to speed up iteration or consts ints
	for (int y = 0; y < flowField.getGridSize().y; y++)
	{
		for (int x = 0; x < flowField.getGridSize().x; x++)
		{
			window.draw(flowField.getGridSquareAt(x, y));
			if (Cell::canShowCostText() == false)
			{
				continue;
			}
			window.draw(flowField.getCostTextAt(x, y));
		}
	}

	window.display();
} 

void Game::update(float deltaTime)
{
	input();
	if (mouseButtonDown)
	{
		if (openPath)
		{
			flowField.setTileAs(PASSABLE, sf::Mouse::getPosition());
			return;
		}
		if (blockPath)
		{
			flowField.setTileAs(IMPASSABLE, sf::Mouse::getPosition());
			return;
		}
	}
	// Update Here....
}

void Game::input()
{
	sf::Event event;

	// while there are pending events...
	while (window.pollEvent(event))
	{
		// check the type of the event...
		switch (event.type)
		{
			// window closed
		case sf::Event::Closed:
			window.close();
			break;

			// key pressed
		case sf::Event::MouseButtonPressed:

			if (event.mouseButton.button == sf::Mouse::Left)
			{
				end = flowField.getCellAtMousePosition(sf::Mouse::getPosition());
				if (end == nullptr)
				{
					break;
				}
				end->setEndColour();
				updateGrid();

				break;
			}

			if (event.mouseButton.button == sf::Mouse::Middle)
			{
				mouseButtonDown = true;

				int myType = flowField.paintType(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
				flowField.setTileAs(myType, sf::Mouse::getPosition());

				if (myType == PASSABLE)
				{
					blockPath = true;
					break;
				}
				if (myType == IMPASSABLE)
				{
					openPath = true;
				}
				break;
			}
			if (event.mouseButton.button == sf::Mouse::Right)
			{
				if (start != nullptr)
				{
					start->setStartingCell(false);
				}
				start = flowField.getCellAtMousePosition(sf::Mouse::getPosition());
				if (start == nullptr)
				{
					break;
				}
				start->setStartColour();
				start->setStartingCell(true);
				updateGrid();
				break;
			}


			break;

		case sf::Event::MouseButtonReleased:

			if (event.mouseButton.button == sf::Mouse::Left)
			{
				break;
			}
			if (event.mouseButton.button == sf::Mouse::Right)
			{

			}
			if (event.mouseButton.button == sf::Mouse::Middle)
			{
				mouseButtonDown = false;
				blockPath = false;
				openPath = false;
				updateGrid();
				break;
			}
			break;


		case sf::Event::KeyPressed:
			if (event.key.code == sf::Keyboard::N)
			{
				flowField.createGrid();
				flowField.setCostField();
				break;
			}
			if (event.key.code == sf::Keyboard::C)
			{
				// Show Tile Cost
				Cell::setShowBest(false);
				for (int y = 0; y < flowField.getGridSize().y; y++)
				{
					for (int x = 0; x < flowField.getGridSize().x; x++)
					{
						flowField.getGrid()[y][x]->switchCostTextDisplay();
					}
				}
			}
			if (event.key.code == sf::Keyboard::B)
			{
				// Show Tile Cost
				Cell::setShowBest(true);
				for (int y = 0; y < flowField.getGridSize().y; y++)
				{
					for (int x = 0; x < flowField.getGridSize().x; x++)
					{
						flowField.getGrid()[y][x]->switchCostTextDisplay();
					}
				}
				break;
			}

			if (event.key.code == sf::Keyboard::T)
			{
				// Show Tile Cost
				if (Cell::canShowCostText() == false)
				{
					Cell::setShowCostText(true);
					break;
				}
				Cell::setShowCostText(false);
				break;
			}
			break;

			
			// we don't process other types of events
		default:
			break;
		}
	}
}

void Game::updateGrid()
{
	flowField.reassignGridValues();
	flowField.setCostField();
	flowField.createIntegrationField(end);
	flowField.createFlowField();
	for (int y = 0; y < flowField.getGridSize().y; y++)
	{
		for (int x = 0; x < flowField.getGridSize().x; x++)
		{
			flowField.getGrid()[y][x]->switchCostTextDisplay();
		}
	}
	flowField.showBestPath(start, end);

}

