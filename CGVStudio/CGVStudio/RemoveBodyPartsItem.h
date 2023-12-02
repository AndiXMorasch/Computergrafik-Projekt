#pragma once

#include "../CGVStudio/Item.h"

using namespace std;

class RemoveBodyPartsItem : public Item
{
public:
	RemoveBodyPartsItem(float radius);
	~RemoveBodyPartsItem() { delete item; };
	void applyItemEffekt(SnakeHead* player);
	void update(float dTime);
	string getItemType();

protected:
	int removeCount = 3;
};