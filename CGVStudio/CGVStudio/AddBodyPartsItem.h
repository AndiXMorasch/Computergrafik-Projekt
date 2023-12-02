#pragma once
#ifndef AddBodyPartsItem_hpp
#define AddBodyPartsItem_hpp
#include "../CGVStudio/Item.h"

using namespace std;

class AddBodyPartsItem : public Item
{
public:
	AddBodyPartsItem(float radius);
	~AddBodyPartsItem() { delete item; };
	void applyItemEffekt(SnakeHead* player);
	void update(float dTime);
	string getItemType();

protected:
	int addCount = 3;
};

#endif