//
//  Application.hpp
//  ogl4
//
//  Created by Philipp Lensing on 16.09.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#ifndef Application_hpp
#define Application_hpp

#include <stdio.h>
#include <list>
#include <random>
#include <cstdlib>
#include <iostream>
#include <time.h>
#include "camera.h"
#include "phongshader.h"
#include "constantshader.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"
#include "basemodel.h"
#include "../CGVStudio/CGVStudio/SnakeHead.h"
#include "../CGVStudio/CGVStudio/Item.h"
#include "../CGVStudio/CGVStudio/FoodItem.h"
#include "../CGVStudio/CGVStudio/SlowItem.h"
#include "../CGVStudio/CGVStudio/SpeedItem.h"
#include "../CGVStudio/CGVStudio/AddBodyPartsItem.h"
#include "../CGVStudio/CGVStudio/RemoveBodyPartsItem.h"
#include "../CGVStudio/CGVStudio/MagnetItem.h"
#include "../CGVStudio/CGVStudio/EarthquakeItem.h"
#include "../CGVStudio/CGVStudio/RotationItem.h"
#include "../CGVStudio/CGVStudio/HurtShader.h"
#include "../CGVStudio/CGVStudio/PortalShader.h"
#include "../CGVStudio/CGVStudio/BloodFrameShader.h"
#include "irrKlang.h"
#include "../CGVStudio/CGVStudio/TextRenderer.h"

using namespace irrklang;
using namespace std;

#pragma comment(lib, "irrKlang.lib")

class Application
{
public:
	typedef std::list<BaseModel*> ModelList;
	Application(GLFWwindow* pWin, ISoundEngine* engine, TextRenderer* tr);
	void start();
	void update(float dTime);
	void draw();
	void drawText();
	void end();

	int playgroundX;
	int playgroundZ;
	float wallHeight;

	TextRenderer* textRenderer;


protected:
	SnakeHead* snakePlayerOne;
	SnakeHead* snakePlayerTwo;
	vector<Item*> items;
	vector<Item*> specialItems;
	vector<Item*> specialItemsOnMap;
	Camera Cam;
	ModelList Models;
	GLFWwindow* pWindow;
	ISoundEngine* engine;
	float time = 0;
	int specialItemsCount = 3;
	float specialItemRespawnTime = 0;
	float random;
	int initialCamRotationTime = 10;
	int countDown;
	// Offscreen Rendering
	//unsigned int framebufferTexture;
	Texture* canvasTex;
	GLuint FBO;
	GLuint RBO;
	OffscreenShader* pOffscreenShader;
	BaseModel* canvas;
	int width, height;
	int NumZonesX;
	int NumZonesZ;
	std::vector<std::vector<std::vector<Item*>>> itemZones;

	void initializeSpecialItemsOnStart(vector<int>& usedItemIndices, int itemCount);
	float getMovementInput(int positiveKey, int negativeKey);
	void initialCameraRotation();
	void readInputsAndUpdateSnakes(float dTime);
	void checkSnakeOneCollision();
	void checkSnakeTwoCollision();
	void checkForTeleportation();
	void updateItems(float dTime);
	void checkForCollisionWithItem();
	void setUnreachablePos(Item* item);
	void randomizePosition(Item* item);
	void eraseSpecialItemFromMap(Item* item);
	void addSpecialItemToMap();
	void checkItemRespawn();
	bool isSnakePartOutOfBounds(BaseModel* snakePart);
};

#endif /* Application_hpp */
