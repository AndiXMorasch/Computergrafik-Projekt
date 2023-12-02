#include "Item.h"

Item::~Item()
{
	delete item;
}

void Item::setShader(BaseShader* baseShader)
{
	item->shader(baseShader, true);
}

bool Item::loadModel(const char* ModelFile)
{
	item = new Model(ModelFile, true);
	item->shader(new PhongShader(true), DeleteShader);

	return true;
}

void Item::moveInDirection(Vector pos, float speed)
{
	Matrix translationMatrix;
	translationMatrix.translation(pos * speed);
	item->transform(item->transform() * translationMatrix);
}

void Item::hover(float dTime) {

	if (item->transform().translation().Y >= maxUp) {
		goingUp = false;
	}
	else if (item->transform().translation().Y <= minUp) {
		goingUp = true;
	}

	Matrix hover;

	if (goingUp) {
		hover.translation(item->transform().translation() + Vector(0, dTime / 3, 0));
	}
	else {
		hover.translation(item->transform().translation() - Vector(0, dTime / 3, 0));
	}

	item->transform(hover);
}

void Item::hoverAndRotate(float dTime) {
	hover(dTime);

	Matrix rotate;
	rotate.rotationY(time);
	item->transform(item->transform() * rotate);
}

bool Item::isDistanceInRange(SnakeHead* target, float distance) {
	Vector dir = target->getPosition() - item->getPosition();
	float dist = dir.lengthSquared(); // Länge bestimmen
	if (dist <= distance) {
		return 1;
	}
	return 0;
}

string Item::getItemType() {
	return itemType;
}

void Item::draw(const BaseCamera& Cam)
{
	item->draw(Cam);
}

float Item::getRadius() {
	return radius;
}

float Item::getMinUp() {
	return minUp;
}

float Item::getMaxUp() {
	return maxUp;
}

Vector Item::getPosition() {
	return item->getPosition();

}
BaseModel* Item::getItem() {
	return this->item;
}