#pragma once
#ifndef SlowItem_hpp
#define SlowItem_hpp

#include "../CGVStudio/Item.h"

using namespace std;

class SlowItem : public Item
{
public:
	SlowItem(float radius);
	~SlowItem() { delete item; };
	void applyItemEffekt(SnakeHead* player);
	void update(float dTime);
	string getItemType();

protected:
	float speedDuration = 5;
};

#endif