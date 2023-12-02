#include "../CGVStudio/MagnetItem.h"

MagnetItem::MagnetItem(float radius) {
	this->goingUp = true;
	this->minUp = radius;
	this->maxUp = radius + 0.5f;
	itemType = "MAGNETITEM";
	item = new TriangleSphereModel(radius, 10, 10);
	this->item->radius = radius;
}

void MagnetItem::update(float dTime) {
	time += dTime;
	hoverAndRotate(dTime);
}

void MagnetItem::applyItemEffekt(SnakeHead* player) {
	player->setMagnetMode();
}

string MagnetItem::getItemType() {
	return itemType;
}

float MagnetItem::getMagnetDistance() {
	return magnetDistance;
}
