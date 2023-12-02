#include "../CGVStudio/SlowItem.h"

SlowItem::SlowItem(float radius) {
	this->goingUp = true;
	this->minUp = radius;
	this->maxUp = radius + 0.5f;
	itemType = "SLOWITEM";
	item = new TriangleSphereModel(radius, 10, 10);
	this->item->radius = radius;
}

void SlowItem::update(float dTime) {
	time += dTime;
	hoverAndRotate(dTime);
}

void SlowItem::applyItemEffekt(SnakeHead* player) {
	player->changeSpeed(false);
}

string SlowItem::getItemType() {
	return itemType;
}