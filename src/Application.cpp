//
//  Application.cpp
//  ogl4
//
//  Created by Philipp Lensing on 16.09.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#define _USE_MATH_DEFINES
#include "Application.h"
#ifdef WIN32
#include <GL/glew.h>
#include <glfw/glfw3.h>
#else
#define GLFW_INCLUDE_GLCOREARB
#define GLFW_INCLUDE_GLEXT
#include <glfw/glfw3.h>
#endif
#include "lineplanemodel.h"
#include "triangleplanemodel.h"
#include "trianglespheremodel.h"
#include "lineboxmodel.h"
#include "triangleboxmodel.h"
#include "model.h"
#include "scene.h"
#include <math.h>
#include <sstream>

#ifdef WIN32
#define ASSET_DIRECTORY "../../assets/"
#else
#define ASSET_DIRECTORY "../assets/"
#endif

Application::Application(GLFWwindow* pWin, ISoundEngine* engine, TextRenderer* tr) : pWindow(pWin), Cam(pWin), engine(engine)
{

	glfwGetFramebufferSize(pWindow, &width, &height);
	BaseModel* pModel;
	HurtShader* pHurtShader;
	PortalShader* pPortalShader;
	ConstantShader* pConstShader;
	PhongShader* pPhongShader;
	pOffscreenShader = new OffscreenShader();

	textRenderer = tr;

	// Define playground size
	playgroundX = 50;
	playgroundZ = 50;

	// Define portal wall height
	wallHeight = 5.0f;

	// Define random number seed
	srand(std::time(0));

	// Skybox laden
	pModel = new Model(ASSET_DIRECTORY "skybox.obj", false);
	pModel->shader(new PhongShader(), true);
	Vector pos = pModel->transform().translation();
	Matrix posMatrix;
	posMatrix.translation(pos - Vector(0, 15, 0));
	pModel->transform(posMatrix);
	Models.push_back(pModel);

	// create LinePlaneModel with constant color shader, zones will be shown
	pModel = new TrianglePlaneModel(playgroundX, playgroundZ, 10, 10);
	pPhongShader = new PhongShader();
	pPhongShader->diffuseColor(Color(1.0f, 1.0f, 1.0f));
	pPhongShader->diffuseTexture(Texture::LoadShared(ASSET_DIRECTORY "sanddune.jpg"));
	pModel->shader(pPhongShader, true);
	Models.push_back(pModel);

	canvas = new TrianglePlaneModel(2, 2, 1, 1);
	canvas->shader(pOffscreenShader, true);

	for (int i = -1; i < 2; i += 2) {
		// create LinePlaneModel for Portalwall in Z
		pModel = new TrianglePlaneModel(playgroundZ, wallHeight, 1, 1);
		pPortalShader = new PortalShader();
		pModel->shader(pPortalShader, true);
		Matrix rotationMatrix, translationMatrix;
		rotationMatrix.rotationX(i * M_PI / 2);
		translationMatrix.translation(Vector(0, wallHeight / 2, -i * playgroundZ / 2));
		pModel->transform(translationMatrix * rotationMatrix);
		Models.push_back(pModel);
	}

	for (int i = -1; i < 2; i += 2) {
		// create LinePlaneModel for Portalwall in X
		pModel = new TrianglePlaneModel(playgroundX, wallHeight, 1, 1);
		pPortalShader = new PortalShader();
		pModel->shader(pPortalShader, true);
		Matrix rotationMatrixX, translationMatrix, rotationMatrixY;
		rotationMatrixX.rotationX(i * M_PI / 2);
		translationMatrix.translation(Vector(-i * playgroundX / 2, wallHeight / 2, 0));
		rotationMatrixY.rotationY(M_PI / 2);
		pModel->transform(translationMatrix * rotationMatrixY * rotationMatrixX);
		Models.push_back(pModel);
	}

	// create SnakeHead Player 1
	Vector spawnPoint(1, 0.5, 10);
	const Texture* textureSnake1 = Texture::LoadShared(ASSET_DIRECTORY "snakeskin1.jpg");
	snakePlayerOne = new SnakeHead(spawnPoint, textureSnake1);
	pPhongShader = new PhongShader();
	pPhongShader->diffuseTexture(textureSnake1);
	snakePlayerOne->setShader(pPhongShader);
	snakePlayerOne->addNewBodyPart();
	snakePlayerOne->addNewBodyPart();
	Models.push_back(snakePlayerOne);

	// create SnakeHead Player 2
	spawnPoint = Vector(1, 0.5, -10);
	const Texture* textureSnake2 = Texture::LoadShared(ASSET_DIRECTORY "snakeskin2.jpg");
	snakePlayerTwo = new SnakeHead(spawnPoint, textureSnake2);
	pPhongShader = new PhongShader();
	pPhongShader->diffuseTexture(textureSnake2);
	snakePlayerTwo->setShader(pPhongShader);
	snakePlayerTwo->addNewBodyPart();
	snakePlayerTwo->addNewBodyPart();
	Models.push_back(snakePlayerTwo);

	float itemSize = 0.6f;

	//create SpeedItem
	Item* speedItem = new SpeedItem(itemSize);
	pPhongShader = new PhongShader();
	speedItem->shader(pPhongShader);
	speedItem->loadModel(ASSET_DIRECTORY "box.dae");
	setUnreachablePos(speedItem);
	items.push_back(speedItem);
	specialItems.push_back(speedItem);

	//create SlowItem
	Item* slowItem = new SlowItem(itemSize);
	pPhongShader = new PhongShader();
	slowItem->shader(pPhongShader, true);
	slowItem->loadModel(ASSET_DIRECTORY "box.dae");
	setUnreachablePos(slowItem);
	items.push_back(slowItem);
	specialItems.push_back(slowItem);

	//create MagnetItem
	Item* magnetItem = new MagnetItem(itemSize);
	pPhongShader = new PhongShader();
	magnetItem->shader(pPhongShader, true);
	magnetItem->loadModel(ASSET_DIRECTORY "box.dae");
	setUnreachablePos(magnetItem);
	items.push_back(magnetItem);
	specialItems.push_back(magnetItem);

	//create AddBodyPartsItem
	Item* addBodyPartsItem = new AddBodyPartsItem(itemSize);
	pPhongShader = new PhongShader();
	addBodyPartsItem->shader(pPhongShader, true);
	addBodyPartsItem->loadModel(ASSET_DIRECTORY "box.dae");
	setUnreachablePos(addBodyPartsItem);
	items.push_back(addBodyPartsItem);
	specialItems.push_back(addBodyPartsItem);

	//create RemoveBodyPartsItem
	Item* removeBodyPartsItem = new RemoveBodyPartsItem(itemSize);
	pPhongShader = new PhongShader();
	removeBodyPartsItem->shader(pPhongShader, true);
	removeBodyPartsItem->loadModel(ASSET_DIRECTORY "box.dae");
	setUnreachablePos(removeBodyPartsItem);
	items.push_back(removeBodyPartsItem);
	specialItems.push_back(removeBodyPartsItem);

	//create EarthquakeItem
	Item* earthquakeItem = new EarthquakeItem(itemSize, Cam);
	pPhongShader = new PhongShader();
	earthquakeItem->shader(pPhongShader, true);
	earthquakeItem->loadModel(ASSET_DIRECTORY "box.dae");
	setUnreachablePos(earthquakeItem);
	items.push_back(earthquakeItem);
	specialItems.push_back(earthquakeItem);

	// create RotationItem
	Item* rotationItem = new RotationItem(itemSize, Cam);
	pPhongShader = new PhongShader();
	rotationItem->shader(pPhongShader, true);
	rotationItem->loadModel(ASSET_DIRECTORY "box.dae");
	setUnreachablePos(rotationItem);
	items.push_back(rotationItem);
	specialItems.push_back(rotationItem);

	vector<int> usedItemIndices;
	initializeSpecialItemsOnStart(usedItemIndices, specialItemsCount - 1);

	for (int i = 0; i < playgroundX + playgroundZ; ++i)
	{
		itemSize = 0.3f;
		Item* foodItem = new FoodItem(itemSize);
		pConstShader = new ConstantShader();
		//pConstShader->color(Color(rand() % 2, rand() % 2, rand() % 2));
		pConstShader->color(Color(1, 1, 1));
		foodItem->setShader(pConstShader);
		randomizePosition(foodItem);
		Models.push_back(foodItem);
		items.push_back(foodItem);
	}

	// Zoning (nur noch optisch)
	// Anzahl der Zonen
	NumZonesX = 2;
	NumZonesZ = 2;
	// Vektor mit Zonen erstellen (X,Z)
	itemZones = std::vector<std::vector<std::vector<Item*>>>(NumZonesX, std::vector<std::vector<Item*>>(NumZonesZ));

	//Items werden in Zones gepackt (X,Z)
	for (Item* item : items)
	{
		//GetPos liefert Position von -25 bis 25, wir brauchen aber position von 0 bis 50
		int zoneX = (int)((item->getPosition().X + playgroundX / 2) / (playgroundX / NumZonesX));
		int zoneZ = (int)((item->getPosition().Z + playgroundZ / 2) / (playgroundZ / NumZonesZ));
		std::cout << "Item in Zone: " << zoneX << " " << zoneZ << std::endl;

		if (item->getItemType() == "FOODITEM") {
			pConstShader = new ConstantShader();
			pConstShader->color(Color(zoneX, zoneZ, 1));
			item->setShader(pConstShader);
		}
		// itemZones[zoneX][zoneZ].push_back(item);
	}
}

void Application::start()
{
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RBO);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	canvasTex = new Texture(width, height, nullptr);
}

void Application::initializeSpecialItemsOnStart(vector<int>& usedItemIndices, int itemCount) {
	bool unusedIntFound = false;
	int size = specialItems.size();

	if (specialItems.size() == usedItemIndices.size()) {
		return;
	}

	while (!unusedIntFound) {
		int randItemPos = rand() % size;
		unusedIntFound = true;

		for (int i : usedItemIndices) {
			if (i == randItemPos) {
				unusedIntFound = false;
				break;
			}
		}

		if (unusedIntFound) {
			randomizePosition(specialItems.at(randItemPos));
			specialItemsOnMap.push_back(specialItems.at(randItemPos));
			usedItemIndices.push_back(randItemPos);
		}
	}

	if (itemCount != 0) {
		initializeSpecialItemsOnStart(usedItemIndices, itemCount - 1);
	}
}

void Application::update(float dTime)
{
	Cam.update();
	time += dTime;
	specialItemRespawnTime -= dTime;
	countDown = 10;
	countDown -= time;

	updateItems(dTime);


	if (time < initialCamRotationTime) {
		initialCameraRotation();
		return;
	}

	readInputsAndUpdateSnakes(dTime);
	checkSnakeOneCollision();
	checkSnakeTwoCollision();
	checkForTeleportation();
	checkForCollisionWithItem();
	checkItemRespawn();
}

void Application::initialCameraRotation() {
	Matrix camRotation, camTranslation, globalCamPos;
	camTranslation.translation((35 + initialCamRotationTime) - time, 35 + initialCamRotationTime - time, 0);
	camRotation.rotationY(-time * 2 * M_PI / initialCamRotationTime);

	globalCamPos = camRotation * camTranslation;
	Vector newPos = globalCamPos.translation();

	Cam.setPosition(newPos);
}

void Application::readInputsAndUpdateSnakes(float dTime) {
	float leftRightOne = getMovementInput(GLFW_KEY_LEFT, GLFW_KEY_RIGHT);
	snakePlayerOne->move(leftRightOne);
	snakePlayerOne->update(dTime);

	float leftRightTwo = getMovementInput(GLFW_KEY_A, GLFW_KEY_D);
	snakePlayerTwo->move(leftRightTwo);
	snakePlayerTwo->update(dTime);
}

float Application::getMovementInput(int positiveKey, int negativeKey)
{
	int pressedPositive = glfwGetKey(pWindow, positiveKey);
	int pressedNegative = glfwGetKey(pWindow, negativeKey);

	float movement = 0.0f;
	if (pressedPositive && !pressedNegative) {
		movement = 1.0f;
	}
	else if (pressedNegative && !pressedPositive) {
		movement = -1.0f;
	}

	return movement;
}

void Application::checkSnakeOneCollision() {
	for (SnakeBody* b : snakePlayerTwo->getBodyParts()) {
		if (snakePlayerOne->checkCollision(b->getBodyPart()) && snakePlayerOne->getSnakeLife() > 0) {
			std::cout << "Ouch, SnakeOne bite me." << std::endl;
			snakePlayerOne->collisionWithPlayerOccurred();
			engine->play2D(ASSET_DIRECTORY "damageSound.mp3", false);
		}
	}
}

void Application::checkSnakeTwoCollision() {
	for (SnakeBody* b : snakePlayerOne->getBodyParts()) {
		if (snakePlayerTwo->checkCollision(b->getBodyPart()) && snakePlayerTwo->getSnakeLife() > 0) {
			std::cout << "Ouch, SnakeTwo bite me." << std::endl;
			snakePlayerTwo->collisionWithPlayerOccurred();
			engine->play2D(ASSET_DIRECTORY "damageSound.mp3", false);
		}
	}
}

void Application::checkForTeleportation() {

	if (isSnakePartOutOfBounds(snakePlayerOne)) {
		Vector oldPos = snakePlayerOne->getPosition();
		Vector newPos = Vector(-oldPos.X, oldPos.Y, -oldPos.Z);
		snakePlayerOne->setTeleportationInProcess(true);
		snakePlayerOne->setNewPosition(newPos);
	}

	if (snakePlayerOne->isTeleportationInProcess()) {
		for (SnakeBody* b : snakePlayerOne->getBodyParts()) {
			if (isSnakePartOutOfBounds(b)) {
				Vector oldPos = b->getPosition();
				Vector newPos = Vector(-oldPos.X, oldPos.Y, -oldPos.Z);
				b->setNewPosition(newPos);
			}
		}

		if (snakePlayerOne->isTeleportationProcessFinished()) {
			std::cout << "Teleportation finished" << std::endl;
			snakePlayerOne->setTeleportationInProcess(false);
		}
	}

	if (isSnakePartOutOfBounds(snakePlayerTwo)) {
		Vector oldPos = snakePlayerTwo->getPosition();
		Vector newPos = Vector(-oldPos.X, oldPos.Y, -oldPos.Z);
		snakePlayerTwo->setTeleportationInProcess(true);
		snakePlayerTwo->setNewPosition(newPos);
	}

	if (snakePlayerTwo->isTeleportationInProcess()) {
		for (SnakeBody* b : snakePlayerTwo->getBodyParts()) {
			if (isSnakePartOutOfBounds(b)) {
				Vector oldPos = b->getPosition();
				Vector newPos = Vector(-oldPos.X, oldPos.Y, -oldPos.Z);
				b->setNewPosition(newPos);
			}
		}

		if (snakePlayerTwo->isTeleportationProcessFinished()) {
			std::cout << "Teleportation finished" << std::endl;
			snakePlayerTwo->setTeleportationInProcess(false);
		}
	}
}

void Application::updateItems(float dTime) {
	for (Item* item : items) {
		item->update(dTime);

		// apply magnet effect if active on player one
		if (snakePlayerOne->isInMagnetMode() && item->isDistanceInRange(snakePlayerOne, 10) && item->getItemType() == "FOODITEM")
		{
			Vector dir = snakePlayerOne->getPosition() - item->getPosition();
			dir.normalize();
			item->moveInDirection(dir, dTime * 5);
		}

		// apply magnet effect if active on player two
		if (snakePlayerTwo->isInMagnetMode() && item->isDistanceInRange(snakePlayerTwo, 10) && item->getItemType() == "FOODITEM")
		{
			Vector dir = snakePlayerTwo->getPosition() - item->getPosition();
			dir.normalize();
			item->moveInDirection(dir, dTime * 5);
		}
	}
}

void Application::checkForCollisionWithItem() {
	for (Item* item : items)
	{
		//Kollision Player 1
		if (snakePlayerOne->checkCollision(item->getItem()))
		{
			//Check if Item is SpeedItem
			if (item->getItemType() == "SPEEDITEM") {
				item->applyItemEffekt(snakePlayerOne);
				std::cout << "SnakeOne SpeedItem" << std::endl;
			}
			//Check if Item is SlowItem
			else if (item->getItemType() == "SLOWITEM") {
				item->applyItemEffekt(snakePlayerTwo);
				std::cout << "SnakeTwo SlowItem" << std::endl;
			}
			//Check if Item is MagnetItem
			else if (item->getItemType() == "MAGNETITEM") {
				item->applyItemEffekt(snakePlayerOne);
				std::cout << "SnakeOne MagnetItem" << std::endl;
			}
			else if (item->getItemType() == "ADDBODYPARTSITEM") {
				item->applyItemEffekt(snakePlayerOne);
				std::cout << "SnakeOne AddBodyPartsItem" << std::endl;
			}
			else if (item->getItemType() == "REMOVEBODYPARTSITEM") {
				item->applyItemEffekt(snakePlayerOne);
				std::cout << "SnakeOne RemoveBodyPartsItem" << std::endl;
			}
			else if (item->getItemType() == "EARTHQUAKEITEM") {
				item->applyItemEffekt();
				std::cout << "SnakeOne EathquakeItem" << std::endl;
			}
			else if (item->getItemType() == "ROTATIONITEM") {
				item->applyItemEffekt();
				std::cout << "SnakeOne RotationItem" << std::endl;
			}
			else {
				// Item neu positionieren und neuen BodyPart hinzufügen
				snakePlayerOne->addFoodCount();
				std::cout << "SnakeOne ate something" << std::endl;
			}

			if (item->getItemType() != "FOODITEM") {
				engine->play2D(ASSET_DIRECTORY "specialItemSound.mp3", false);
				specialItemRespawnTime = rand() % 20;
				eraseSpecialItemFromMap(item);
			}
			else {
				engine->play2D(ASSET_DIRECTORY "foodItemSound.mp3", false);
				randomizePosition(item);
			}
		}

		//Kollision Player 2
		if (snakePlayerTwo->checkCollision(item->getItem()))
		{
			//Check if Item is SpeedItem
			if (item->getItemType() == "SPEEDITEM") {
				item->applyItemEffekt(snakePlayerTwo);
				std::cout << "SnakeTwo SpeedItem" << std::endl;
			}
			//Check if Item is SlowItem
			else if (item->getItemType() == "SLOWITEM") {
				item->applyItemEffekt(snakePlayerOne);
				std::cout << "SnakeOne NegativeItem" << std::endl;
			}
			//Check if Item is MagnetItem
			else if (item->getItemType() == "MAGNETITEM") {
				item->applyItemEffekt(snakePlayerTwo);
				std::cout << "SnakeTwo MagnetItem" << std::endl;
			}
			else if (item->getItemType() == "ADDBODYPARTSITEM") {
				item->applyItemEffekt(snakePlayerTwo);
				std::cout << "SnakeTwo AddBodyPartItem" << std::endl;
			}
			else if (item->getItemType() == "REMOVEBODYPARTSITEM") {
				item->applyItemEffekt(snakePlayerTwo);
				std::cout << "SnakeOne RemoveBodyPartsItem" << std::endl;
			}
			else if (item->getItemType() == "EARTHQUAKEITEM") {
				item->applyItemEffekt();
				std::cout << "SnakeOne EathquakeItem" << std::endl;
			}
			else if (item->getItemType() == "ROTATIONITEM") {
				item->applyItemEffekt();
				std::cout << "SnakeOne RotationItem" << std::endl;
			}
			else {
				// Item neu positionieren und neuen BodyPart hinzufügen
				snakePlayerTwo->addFoodCount();
				std::cout << "SnakeTwo ate something" << std::endl;
			}

			if (item->getItemType() != "FOODITEM") {
				engine->play2D(ASSET_DIRECTORY "specialItemSound.mp3", false);
				specialItemRespawnTime = rand() % 20;
				std::cout << specialItemRespawnTime << std::endl;
				eraseSpecialItemFromMap(item);
			}
			else {
				engine->play2D(ASSET_DIRECTORY "foodItemSound.mp3", false);
				randomizePosition(item);
			}
		}
	}
}

void Application::setUnreachablePos(Item* item) {
	Matrix targetPos;
	targetPos.translation(playgroundX * 2, -99, playgroundZ * 2);
	item->getItem()->transform(targetPos);
}

void Application::randomizePosition(Item* item) {

	float randomX = rand() % (playgroundX - 1) - ((playgroundX - 1) / 2.0f);
	float randomY = item->getMinUp();
	if (rand() % 2 == 1) {
		randomY = item->getMaxUp();
	}
	float randomZ = rand() % (playgroundZ - 1) - ((playgroundZ - 1) / 2.0f);

	Matrix targetPos;
	targetPos.translation(randomX, randomY, randomZ);
	item->getItem()->transform(targetPos);
}

void Application::eraseSpecialItemFromMap(Item* item) {
	int index1 = -1;
	for (int i = 0; i < specialItemsOnMap.size(); i++) {
		if (item->getItemType() == specialItemsOnMap.at(i)->getItemType()) {
			index1 = i;
		}
	}

	if (index1 != -1) {
		setUnreachablePos(item);
		specialItemsOnMap.erase(specialItemsOnMap.begin() + index1);
	}

	Matrix neutralMatrix;
	item->getItem()->transform(neutralMatrix);
}

void Application::checkItemRespawn() {
	//std::cout << "Respawn Time: " << specialItemRespawnTime << std::endl;
	if (specialItemsOnMap.size() < specialItemsCount && specialItemRespawnTime < 0) {
		addSpecialItemToMap();

		if (specialItemsOnMap.size() < specialItemsCount) {
			specialItemRespawnTime = rand() % 20;
			std::cout << specialItemRespawnTime << std::endl;
		}
	}
}

void Application::addSpecialItemToMap() {
	bool unusedIntFound = false;
	int size = specialItems.size();

	if (specialItems.size() == specialItemsOnMap.size()) {
		return;
	}

	while (!unusedIntFound) {
		unusedIntFound = true;
		int index = rand() % size;
		Item* item = specialItems.at(index);

		for (Item* i : specialItemsOnMap) {
			if (item->getItemType() == i->getItemType()) {
				unusedIntFound = false;
				break;
			}
		}

		if (unusedIntFound) {
			Item* item = specialItems.at(index);
			randomizePosition(item);
			specialItemsOnMap.push_back(item);
		}
	}
}

bool Application::isSnakePartOutOfBounds(BaseModel* snakePart) {
	float posX = snakePart->getPosition().X;
	float posZ = snakePart->getPosition().Z;

	if (posZ > (playgroundZ / 2) || posX > (playgroundX / 2) || posZ < (-playgroundZ / 2) || posX < (-playgroundX / 2)) {
		return true;
	}

	return false;
}

void Application::draw()
{
	if (snakePlayerOne->didCollisionJustHappened() || snakePlayerTwo->didCollisionJustHappened()) {
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, canvasTex->getTextureId(), 0);
	}

	// 1. clear screen
	glClearColor(255, 255, 0, 255);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (Item* i : specialItemsOnMap) {
		i->draw(Cam);
	}

	// 2. setup shaders and draw models
	for (ModelList::iterator it = Models.begin(); it != Models.end(); ++it)
	{
		(*it)->draw(Cam);
	}

	drawText();

	if (snakePlayerOne->didCollisionJustHappened() || snakePlayerTwo->didCollisionJustHappened()) {
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glClearColor(0, 0, 0, 0);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		pOffscreenShader->setTexture(canvasTex);
		pOffscreenShader->activate(Cam);

		canvas->draw(Cam);
	}

	// 3. check once per frame for opengl errors
	GLenum Error = glGetError();
	//assert(Error == 0);
	// Hier tritt immer ein Debug Error auf...
}

void Application::drawText()
{
	// textRenderer.RenderText("Text to render", x, y, scale, color);
	if (countDown > 0) {
		textRenderer->RenderText("Game starts in", 240.f, 400.0f, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
		textRenderer->RenderText(std::to_string(countDown), 390.f, 350.0f, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
	}
	else {
		textRenderer->RenderText("Snake 1", 20.0f, 550.0f, 0.5f, glm::vec3(0.1f, 0.1f, 0.1f));
		textRenderer->RenderText(std::to_string(snakePlayerOne->getSnakeLife()), 20.0f, 500.0f, 1.0f, glm::vec3(1.0f, 0.1f, 0.1f));
		textRenderer->RenderText("Snake 2", 690.0f, 550.0f, 0.5f, glm::vec3(0.1f, 0.1f, 0.1f));
		textRenderer->RenderText(std::to_string(snakePlayerTwo->getSnakeLife()), 755.0f, 500.0f, 1.0f, glm::vec3(1.0f, 0.1f, 0.1f));

		if (snakePlayerOne->getSnakeLife() == 0 || snakePlayerTwo->getSnakeLife() == 0) {
			textRenderer->RenderText("Game Over", 260.f, 300.0f, 1.0f, glm::vec3(1.0f, 0.0f, 0.0f));
			snakePlayerOne->setSnakeSpeed(0);
			snakePlayerTwo->setSnakeSpeed(0);

			if (snakePlayerOne->getSnakeLife() == 0) {
				textRenderer->RenderText("Snake 2 wins!", 315.f, 270.0f, 0.5f, glm::vec3(1.0f, 1.0f, 1.0f));
			}

			if (snakePlayerTwo->getSnakeLife() == 0) {
				textRenderer->RenderText("Snake 1 wins!", 315.f, 270.0f, 0.5f, glm::vec3(1.0f, 1.0f, 1.0f));
			}
		}
	}
}

void Application::end()
{
	for (ModelList::iterator it = Models.begin(); it != Models.end(); ++it)
		delete* it;

	Models.clear();
}