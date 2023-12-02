#include "../CGVStudio/RemoveBodyPartsItem.h"

RemoveBodyPartsItem::RemoveBodyPartsItem(float radius) {
	this->goingUp = true;
	this->minUp = radius;
	this->maxUp = radius + 0.5f;
	itemType = "REMOVEBODYPARTSITEM";
	item = new TriangleSphereModel(radius, 10, 10);
	this->item->radius = radius;
}

void RemoveBodyPartsItem::update(float dTime) {
	time += dTime;
	hoverAndRotate(dTime);
}

void RemoveBodyPartsItem::applyItemEffekt(SnakeHead* player) {
	for (int i = 0; i < removeCount; i++) {
		player->removeBodyPart();
	}
}

string RemoveBodyPartsItem::getItemType() {
	return itemType;
}