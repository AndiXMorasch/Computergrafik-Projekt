#pragma once
#ifndef MagnetItem_hpp
#define MagnetItem_hpp

#include "../CGVStudio/Item.h"

using namespace std;

class MagnetItem : public Item
{
public:
	MagnetItem(float radius);
	~MagnetItem() { delete item; };
	void applyItemEffekt(SnakeHead* player);
	void update(float dTime);
	string getItemType();
	float getMagnetDistance();

protected:
	float magnetDistance = 10;
	float magnetDuration = 5;
};

#endif