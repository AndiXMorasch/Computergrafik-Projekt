#include "../CGVStudio/AddBodyPartsItem.h"

AddBodyPartsItem::AddBodyPartsItem(float radius) {
	this->goingUp = true;
	this->minUp = radius;
	this->maxUp = radius + 0.5f;
	itemType = "ADDBODYPARTSITEM";
	item = new TriangleSphereModel(radius, 10, 10);
	this->item->radius = radius;
}

void AddBodyPartsItem::update(float dTime) {
	time += dTime;
	hoverAndRotate(dTime);
}

void AddBodyPartsItem::applyItemEffekt(SnakeHead* player) {
	for (int i = 0; i < addCount; i++) {
		player->addNewBodyPart();
	}
}

string AddBodyPartsItem::getItemType() {
	return itemType;
}