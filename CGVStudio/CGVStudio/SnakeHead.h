#pragma once
#include "SnakeBody.h"
#include "../../src/BaseModel.h"
#include "../../src/Model.h"
#include "../../src/TriangleSphereModel.h"
#include "../../src/ConstantShader.h"
#include "../../src/PhongShader.h"
#include "HurtShader.h"

#ifdef WIN32
#define ASSET_DIRECTORY "../../assets/"
#else
#define ASSET_DIRECTORY "../assets/"
#endif

using namespace std;

class SnakeHead : public BaseModel
{
public:
	SnakeHead(Vector spawnPoint, const Texture* texture);
	virtual ~SnakeHead();
	void initializeSnake(Vector pos);
	void move(float leftRight);
	void setNewPosition(Vector pos);
	void setShader(BaseShader* baseShader);
	void update(float dtime);
	void addNewBodyPart();
	void removeBodyPart();
	virtual void draw(const BaseCamera& Cam);
	bool checkCollision(BaseModel* target);
	void collisionWithPlayerOccurred();
	Vector getPosition() { return head->getPosition(); };
	void changeSpeed(bool faster);
	void addFoodCount() { foodCount = foodCount + 1; };
	void setMagnetMode();
	bool isInMagnetMode();
	bool isTeleportationInProcess();
	void setTeleportationInProcess(bool teleportationInProcess);
	bool isTeleportationProcessFinished();
	bool didCollisionJustHappened() { return collisionJustHappened; };
	vector<SnakeBody*> getBodyParts();
	int snakeLife = 3;
	void setSnakeLife(int life) { if (snakeLife > 0) { this->snakeLife = life; } };
	int getSnakeLife(){ return snakeLife; };
	void setSnakeSpeed(float speed) { this->speed = speed; };

protected:
	BaseModel* head;
	const Texture* texture;
	float time;
	float radius;
	float speedDuration;
	bool changedSpeedMode = false;
	float magnetDuration;
	bool magnetMode = false;
	float noCollisionOccuredTimer;
	bool collisionJustHappened = false;
	float leftRight;
	Vector forward;
	Vector right;
	float speed;
	float normalSpeed = 7.0f;
	bool teleportationInProcess;
	int foodCount;
	vector<SnakeBody*> bodyParts;
	float rotationAngle = 0;

private:
	void resetShaderToNormal();
};

