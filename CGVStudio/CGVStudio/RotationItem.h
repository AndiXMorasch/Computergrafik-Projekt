#pragma once
#ifndef RotationItem_hpp
#define RotationItem_hpp

#define _USE_MATH_DEFINES

#include "../CGVStudio/Item.h"
#include "../../src/Camera.h"
#include <math.h>

using namespace std;

class RotationItem : public Item
{
public:
	RotationItem(float radius, Camera& cam);
	~RotationItem() { delete item; };
	void applyItemEffekt();
	void update(float dTime);
	bool isRotationActive() { return rotationOngoing; };
	string getItemType();

protected:
	Camera& cam;
	Vector initTranslation;
	bool rotationOngoing;
	float rotationTimePassed = 0.0f;
	float rotationDurationForQuarter = 1.0f;
	void rotateCamera(float dTime);
};

#endif
