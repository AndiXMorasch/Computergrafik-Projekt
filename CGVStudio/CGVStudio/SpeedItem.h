#pragma once
#ifndef SpeedItem_hpp
#define SpeedItem_hpp

#include "../CGVStudio/Item.h"

using namespace std;

class SpeedItem : public Item
{
public:
	SpeedItem(float radius);
	virtual ~SpeedItem() { delete item; };
	void applyItemEffekt(SnakeHead* player);
	void update(float dTime);
	string getItemType();

protected:
	float speedDuration = 2;
};

#endif;
