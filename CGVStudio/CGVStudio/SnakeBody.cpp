#include "SnakeBody.h"

SnakeBody::SnakeBody(BaseModel* target, float speed) {
	radius = 0.5f;
	body = new TriangleSphereModel(radius, 18, 36);
	this->speed = speed;
	time = 0;
	this->target = target;
	initializeSnakeBody();
}

void SnakeBody::initializeSnakeBody() {
	Matrix bodyPos;
	bodyPos.translation(target->getPosition().X - 1, radius, target->getPosition().Z);
	body->transform(bodyPos);
}

void SnakeBody::setNewPosition(Vector pos) {
	Matrix newPosMatrix;
	newPosMatrix.translation(pos);
	body->transform(newPosMatrix);
	body->setTeleported(true);
	target->setTeleported(false);
}

SnakeBody::~SnakeBody() {
	delete body;
}

void SnakeBody::update(float dTime) {

	if (!target->isTeleported()) {
		Vector up = body->transform().up();
		Vector right = target->getPosition() - body->transform().translation();
		right.normalize();
		forward = right.cross(up);
		forward.normalize();
		Vector translation = target->getPosition() - right;

		Matrix matrix;
		matrix.translation(translation);
		matrix.up(up);
		matrix.forward(forward);
		matrix.right(right);
		body->transform(matrix);
	}
	else {
		Matrix translationMatrix;
		translationMatrix.translation(dTime * speed, 0, 0);
		body->transform(body->transform() * translationMatrix);
	}
}

BaseModel* SnakeBody::getBodyPart() {
	return this->body;
}

Vector SnakeBody::getPosition() {
	return body->getPosition();
}

void SnakeBody::setShader(BaseShader* baseShader) {
	body->shader(baseShader, true);
}

void SnakeBody::draw(const BaseCamera& Cam) {
	body->draw(Cam);
}