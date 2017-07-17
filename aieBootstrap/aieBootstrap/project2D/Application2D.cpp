#include "Application2D.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include "Defines.h"
#include "GridNode.h"

using namespace aie;



Application2D::Application2D() 
{
}

Application2D::~Application2D() 
{
}

bool Application2D::startup() 
{
	m_2dRenderer = new Renderer2D();

	m_shipTexture = new Texture("./textures/ship.png");

	m_font = new Font("./font/consolas.ttf", 32);

	m_audio = new Audio("./audio/powerup.wav");

	m_cameraX = -25;
	m_cameraY = -25;
	m_timer = 0;

	///////////////////////////////////////////////
	// CREATES GRID ON SCREEN
	//////////////////////////////////////////////
	m_ppGrid = new GridNode*[GRID_SIZE * GRID_SIZE];

	for (int x = 0; x < GRID_SIZE; ++x)
	{
		for (int y = 0; y < GRID_SIZE; ++y)
		{
			//Calcuate the index of the node from the x and y.
			int index = (y * GRID_SIZE) + x;

			//Calcuate position of node in game world.
			Vector2 pos(x * NODE_SIZE, y * NODE_SIZE);

			//create the node.
			m_ppGrid[index] = new GridNode(pos, x, y);
		}
	}

	//Connect up adjacent nodes
	for (int x = 0; x < GRID_SIZE; ++x)
	{
		for (int y = 0; y < GRID_SIZE; ++y)
		{
			int index = (y * GRID_SIZE) + x;
			GridNode * currentNode = m_ppGrid[index];

			//Adjacent nodes
			//---------------
			//|   | 3 |   |
			//---------------
			//| 0 | C | 2 | 
			//---------------
			//|   | 1 |   |
			//---------------


			for (int a = 0; a < 4; ++a)
			{
				int localX = x;
				int localY = y;

				if (a % 2 == 0)
					localX += - 1;
				else
					localY += - 2;

				if (localX < 0 || localX >= GRID_SIZE)
					continue;
				if (localY < 0 || localY >= GRID_SIZE)
					continue;

				int localIndex = (localY * GRID_SIZE) + localX;
				GridNode* adjNode = m_ppGrid[localIndex];

				//connects adjacency

				AstarEdge* pEdge = new AstarEdge();
				pEdge->m_pEndNode = adjNode;
				pEdge->m_nCost = 10;

				currentNode->m_AdjacentList.push_back(pEdge);
			}

			//Diagnal nodes
			for (int d = 0; d < 4; ++d)
			{

			}
		}
	}

	return true;
}

void Application2D::shutdown() 
{

	for (int i = 0; i < GRID_SIZE * GRID_SIZE; ++i)
	{
		delete m_ppGrid[i];
	}

	delete[] m_ppGrid;
	delete m_audio;
	delete m_font;
	delete m_shipTexture;
	delete m_2dRenderer;
}

void Application2D::update(float deltaTime) 
{
	m_timer += deltaTime;

	// input example
	Input* input = Input::getInstance();

	// use arrow keys to move camera
	if (input->isKeyDown(INPUT_KEY_UP))
		m_cameraY += 500.0f * deltaTime;

	if (input->isKeyDown(INPUT_KEY_DOWN))
		m_cameraY -= 500.0f * deltaTime;

	if (input->isKeyDown(INPUT_KEY_LEFT))
		m_cameraX -= 500.0f * deltaTime;

	if (input->isKeyDown(INPUT_KEY_RIGHT))
		m_cameraX += 500.0f * deltaTime;

	// example of audio
	if (input->wasKeyPressed(INPUT_KEY_SPACE))
		m_audio->play();

	// exit the application
	if (input->isKeyDown(INPUT_KEY_ESCAPE))
		quit();
}

void Application2D::draw() 
{
	// wipe the screen to the background colour
	clearScreen();

	// set the camera position before we begin rendering
	m_2dRenderer->setCameraPos(m_cameraX, m_cameraY);

	// begin drawing sprites
	m_2dRenderer->begin();

	for (int i = 0; i < GRID_SIZE * GRID_SIZE; ++i)
	{
		float x = m_ppGrid[i]->m_v2Pos.x;
		float y = m_ppGrid[i]->m_v2Pos.y;
		m_2dRenderer->drawBox(x, y, NODE_SIZE - EDGE_SPACING, NODE_SIZE - EDGE_SPACING);

		//Draw adjacency
		for (int a = 0; a < m_ppGrid[i]->m_AdjacentList.size(); ++a)
		{
			GridNode* otherNode ((GridNode*)m_ppGrid[i]->m_AdjacentList[a]->m_pEndNode);

			float otherX = otherNode->m_v2Pos.x;
			float otherY = otherNode->m_v2Pos.y;
			m_2dRenderer->setRenderColour(0xFF0000FF);
			m_2dRenderer->drawLine(x, y, otherX, otherY, 2.0f);
			m_2dRenderer->setRenderColour(0xFFFFFFFF);
		}
	}

	// output some text, uses the last used colour
	char fps[32];
	sprintf_s(fps, 32, "FPS: %i", getFPS());
	m_2dRenderer->drawText(m_font, fps, 0, 720 - 32);

	// done drawing sprites
	m_2dRenderer->end();
}