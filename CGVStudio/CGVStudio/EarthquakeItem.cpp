#include "../CGVStudio/EarthquakeItem.h"

EarthquakeItem::EarthquakeItem(float radius, Camera& cam) : cam(cam) {
	this->goingUp = true;
	this->minUp = radius;
	this->maxUp = radius + 0.5f;
	this->earthquakeOngoing = false;
	itemType = "EARTHQUAKEITEM";
	item = new TriangleSphereModel(radius, 10, 10);
	this->item->radius = radius;
}

void EarthquakeItem::update(float dTime) {
	time += dTime;
	if (earthquakeDuration < 0 && earthquakeOngoing) {
		earthquakeOngoing = false;
		cam.setPosition(oldCamPos);
		cam.setTarget(oldTarget);
		std::cout << "ERDBEBEN HÖRT AUF" << std::endl;
	}
	else if (earthquakeOngoing) {
		earthquakeDuration -= dTime;
		shakeCamera(dTime);
	}

	hoverAndRotate(dTime);
}

void EarthquakeItem::applyItemEffekt() {
	std::cout << "ERDBEBEN FÄNGT AN" << std::endl;
	oldCamPos = cam.position();
	earthquakeOngoing = true;
	earthquakeDuration = 5.0f;
}

// Erdbeben Effekt
void EarthquakeItem::shakeCamera(float dTime) {

	if (cam.position().Y >= (oldCamPos.Y + 2)) {
		camShakeUp = false;
	}
	else if (cam.position().Y <= (oldCamPos.Y - 2)) {
		camShakeUp = true;
	}

	Vector newCamPosition;
	Vector moveDistance = Vector(0, dTime * 120, 0);

	if (camShakeUp) {
		newCamPosition = cam.position() + moveDistance + Vector(0, 0, dTime * 100);
		cam.setTarget(cam.target() - moveDistance);
	}
	else {
		newCamPosition = cam.position() - moveDistance - Vector(0, 0, dTime * 100);
		cam.setTarget(cam.target() + moveDistance);
	}

	cam.setPosition(newCamPosition);
}

string EarthquakeItem::getItemType() {
	return itemType;
}