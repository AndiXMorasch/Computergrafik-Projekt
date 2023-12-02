#include "../CGVStudio/FoodItem.h"

FoodItem::FoodItem(float radius) {
	this->goingUp = true;
	this->minUp = radius;
	this->maxUp = radius + 0.5f;
	itemType = "FOODITEM";
	item = new TriangleSphereModel(radius, 10, 10);
	this->item->radius = radius;
}

void FoodItem::update(float dTime) {
	time += dTime;
	hover(dTime);
}

string FoodItem::getItemType() {
	return itemType;
}