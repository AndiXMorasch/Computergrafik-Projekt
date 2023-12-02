#pragma once
#ifndef Item_hpp
#define Item_hpp

#include "../../src/BaseModel.h"
#include "../CGVStudio/SnakeHead.h"
#include "../../src/ConstantShader.h"

using namespace std;

class Item : public Model
{
public:
	Item() { itemType = "BASEITEM"; };
	virtual ~Item();
	void setShader(BaseShader* baseShader);
	virtual void update(float dTime) {};
	bool loadModel(const char* modelFile);
	virtual void applyItemEffekt(SnakeHead* player) {};
	virtual void applyItemEffekt() {};
	void hover(float dTime);
	void hoverAndRotate(float dTime);
	void moveInDirection(Vector pos, float dtime);
	bool isDistanceInRange(SnakeHead* target, float distance);
	virtual string getItemType();
	virtual void draw(const BaseCamera& Cam);

	float getRadius();
	float getMinUp();
	float getMaxUp();
	BaseModel* getItem();
	Vector getPosition();

protected:
	BaseModel* item;
	float time = 0;
	float minUp;
	float maxUp;
	bool goingUp;
	string itemType;
};

#endif


