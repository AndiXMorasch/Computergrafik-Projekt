#pragma once
#ifndef EarthquakeItem_hpp
#define EarthquakeItem_hpp

#include "../CGVStudio/Item.h"
#include "../../src/Camera.h"

using namespace std;

class EarthquakeItem : public Item
{
public:
	EarthquakeItem(float radius, Camera& cam);
	~EarthquakeItem() { delete item; };
	void applyItemEffekt();
	void update(float dTime);
	bool isEathquakeActive() { return earthquakeOngoing; };
	string getItemType();

protected:
	Camera& cam;
	Vector oldCamPos;
	Vector oldTarget = Vector(0, 0, 0);
	bool camShakeUp = false;
	bool earthquakeOngoing;
	float earthquakeDuration = 5.0f;
	void shakeCamera(float dTime);
};

#endif
