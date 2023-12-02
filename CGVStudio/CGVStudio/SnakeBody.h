#pragma once
#include "../../src/BaseModel.h"
#include "../../src/TriangleSphereModel.h"
#include "../../src/ConstantShader.h"

class SnakeBody : public BaseModel
{
public:
	SnakeBody(BaseModel* target, float speed);
	virtual ~SnakeBody();
	void initializeSnakeBody();
	void setShader(BaseShader* baseShader);
	Vector getPosition();
	void setNewPosition(Vector pos);
	void update(float dTime);
	BaseModel* getBodyPart();
	virtual void draw(const BaseCamera& Cam);
	float getRadius() { return radius; }
protected:
	float radius;
	float speed;
	BaseModel* body;
	BaseModel* target;
	Vector forward;
	float time;
};

