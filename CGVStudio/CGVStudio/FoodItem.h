#pragma once
#ifndef FoodItem_hpp
#define FoodItem_hpp

#include "../CGVStudio/Item.h"

using namespace std;

class FoodItem : public Item
{
public:
	FoodItem(float radius);
	virtual ~FoodItem() { delete item; };
	void update(float dTime);
	string getItemType();
};

#endif