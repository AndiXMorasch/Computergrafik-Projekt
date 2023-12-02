#include "SnakeHead.h"

SnakeHead::SnakeHead(Vector spawnPoint, const Texture* texture) {
	this->texture = texture;
	radius = 0.5f;
	magnetDuration, speedDuration, time, foodCount = 0;
	speed = normalSpeed;
	magnetMode = false;
	teleportationInProcess = false;
	initializeSnake(spawnPoint);
}

void SnakeHead::initializeSnake(Vector pos) {
	head = new TriangleSphereModel(radius, 18, 36);

	Matrix snakePos;
	snakePos.translation(pos);
	head->transform(snakePos);
}

SnakeHead::~SnakeHead() {
	delete head;
}

void SnakeHead::move(float LeftRight) {
	leftRight = LeftRight;
}

void SnakeHead::setNewPosition(Vector pos) {
	Matrix newPosMatrix, rotationMatrix;
	newPosMatrix.translation(pos);
	rotationMatrix.rotationY(rotationAngle);
	head->transform(newPosMatrix * rotationMatrix);
	teleportationInProcess = true;
	head->setTeleported(true);
}

void SnakeHead::update(float dTime) {
	time += dTime;
	speedDuration -= dTime;
	magnetDuration -= dTime;
	noCollisionOccuredTimer -= dTime;

	Matrix translationMatrix, rotationMatrix;

	// Rotiere um Y
	rotationMatrix.rotationY(leftRight * dTime * 2);
	rotationAngle += leftRight * dTime * 2;
	head->transform(head->transform() * rotationMatrix);

	// Fahre geradeaus wohin du schaust
	translationMatrix.translation(dTime * speed, 0, 0);
	head->transform(head->transform() * translationMatrix);

	for (SnakeBody* b : bodyParts) {
		b->update(dTime);
	}

	if (foodCount == 5) {
		std::cout << "Foodcount: " << foodCount << std::endl;
		foodCount = 0;
		this->addNewBodyPart();
	}

	if (speedDuration < 0 && changedSpeedMode) {
		speed = normalSpeed;
		changedSpeedMode = false;
	}

	if (magnetDuration < 0 && magnetMode) {
		magnetMode = false;
	}

	if (noCollisionOccuredTimer < 0 && collisionJustHappened) {
		collisionJustHappened = false;
		resetShaderToNormal();
	}

	leftRight = 0;
}

void SnakeHead::draw(const BaseCamera& Cam) {
	head->draw(Cam);

	for (SnakeBody* b : bodyParts) {
		b->draw(Cam);
	}
}

void SnakeHead::resetShaderToNormal() {
	PhongShader* pPhongShader = new PhongShader();
	pPhongShader->diffuseTexture(texture);
	setShader(pPhongShader);

	for (SnakeBody* b : bodyParts) {
		PhongShader* pPhongShader = new PhongShader();
		pPhongShader->diffuseTexture(texture);
		b->setShader(pPhongShader);
	}
}

void SnakeHead::changeSpeed(bool faster) {
	if (faster) {
		this->speed *= 2;
	}
	else {
		this->speed /= 2;
	}

	speedDuration = 5.0f;
	changedSpeedMode = true;
}

void SnakeHead::setMagnetMode() {
	this->magnetMode = true;
	this->magnetDuration = 5.0f;
}

bool SnakeHead::isInMagnetMode() {
	return magnetMode;
}

void SnakeHead::addNewBodyPart() {

	SnakeBody* newSnakeBody = new SnakeBody(this->head, normalSpeed);

	if (!bodyParts.empty()) {
		newSnakeBody = new SnakeBody(bodyParts.at(bodyParts.size() - 1)->getBodyPart(), normalSpeed);
	}

	PhongShader* shader = new PhongShader(true);
	shader->diffuseTexture(texture);
	newSnakeBody->setShader(shader);

	bodyParts.push_back(newSnakeBody);
}

void SnakeHead::removeBodyPart()
{
	if (!bodyParts.empty()) {
		bodyParts.pop_back();
	}
}

vector<SnakeBody*> SnakeHead::getBodyParts() {
	return bodyParts;
}

bool SnakeHead::isTeleportationInProcess() {
	return teleportationInProcess;
}

void SnakeHead::setTeleportationInProcess(bool teleportationInProcess) {
	if (speed != normalSpeed) {
		speed = normalSpeed;
	}

	this->teleportationInProcess = teleportationInProcess;
}

bool SnakeHead::isTeleportationProcessFinished() {
	// Wenn SnakeHead selbst noch teleported ist, dann ist der Prozess
	// sowieso noch nicht abgeschlossen
	if (head->isTeleported()) {
		return false;
	}

	bool othersTeleported = true;
	for (int i = 0; i < bodyParts.size() - 1; i++) {
		if (bodyParts.at(i)->getBodyPart()->isTeleported()) {
			othersTeleported = false;
		}
	}

	// Wenn der letzte sich teleportiert hat und der teleported status
	// aller anderen auf false ist (weil die sich bereits teleportiert haben)
	// dann ist der Prozess abgeschlossen
	if (othersTeleported && bodyParts.back()->getBodyPart()->isTeleported()) {
		bodyParts.back()->getBodyPart()->setTeleported(false);
		return true;
	}

	return false;
}

void SnakeHead::setShader(BaseShader* baseShader) {
	head->shader(baseShader, true);
}

//Quelle: https://stackoverflow.com/questions/64083503/proper-sphere-collision-resolution-in-c
bool SnakeHead::checkCollision(BaseModel* target) {
	Vector dir = head->getPosition() - target->getPosition();
	float dist = dir.lengthSquared();
	if (dist <= radius + target->getRadius()) {
		std::cout << "Target hat Radius von: " << target->getRadius() << std::endl;
		return 1;
	}
	return 0;
}

void SnakeHead::collisionWithPlayerOccurred() {
	if (!collisionJustHappened) {
		HurtShader* pHurtShader = new HurtShader();
		pHurtShader->diffuseTexture(texture);
		setShader(pHurtShader);
		this->setSnakeLife(--snakeLife);

		for (SnakeBody* b : bodyParts) {
			HurtShader* pHurtShader = new HurtShader();
			pHurtShader->diffuseTexture(texture);
			b->setShader(pHurtShader);
		}
	}

	collisionJustHappened = true;
	noCollisionOccuredTimer = 1.0f;
}