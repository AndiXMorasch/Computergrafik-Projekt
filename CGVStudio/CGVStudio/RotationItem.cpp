#include "../CGVStudio/RotationItem.h"

RotationItem::RotationItem(float radius, Camera& cam) : cam(cam) {
	this->minUp = radius;
	this->maxUp = radius + 0.5f;
	this->rotationOngoing = false;
	itemType = "ROTATIONITEM";
	item = new TriangleSphereModel(radius, 10, 10);
	this->item->radius = radius;
}

void RotationItem::update(float dTime) {
	time += dTime;
	if (rotationDurationForQuarter < 0 && rotationOngoing) {
		rotationOngoing = false;
		std::cout << "ROTATION HÖRT AUF" << std::endl;
	}
	else if (rotationOngoing) {
		rotationDurationForQuarter -= dTime;
		rotateCamera(dTime);
	}

	hoverAndRotate(dTime);
}

void RotationItem::applyItemEffekt() {
	std::cout << "ROTATION FÄNGT AN" << std::endl;
	rotationOngoing = true;
	rotationDurationForQuarter = 1.0f;
	initTranslation = Vector(cam.position().X, cam.position().Y, cam.position().Z);
	rotationTimePassed = 0.0f;
}

// Rotation Effekt
void RotationItem::rotateCamera(float dTime) {

	rotationTimePassed += dTime;

	Matrix camRotation, camTranslation;
	camTranslation.translation(initTranslation);
	camRotation.rotationY(rotationTimePassed * 2 * M_PI * 1 / 4);

	Matrix globalMatrix = camRotation * camTranslation;
	Vector newPos = globalMatrix.translation();

	cam.setPosition(newPos);
}

string RotationItem::getItemType() {
	return itemType;
}