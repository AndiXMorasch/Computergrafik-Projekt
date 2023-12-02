#include "../CGVStudio/SpeedItem.h"

SpeedItem::SpeedItem(float radius) {
	this->goingUp = true;
	this->minUp = radius;
	this->maxUp = radius + 0.5f;
	itemType = "SPEEDITEM";
	item = new TriangleSphereModel(radius, 10, 10);
	this->item->radius = radius;
}

void SpeedItem::update(float dTime) {
	time += dTime;
	hoverAndRotate(dTime);
}

void SpeedItem::applyItemEffekt(SnakeHead* player) {
	player->changeSpeed(true);
}

string SpeedItem::getItemType() {
	return itemType;
}