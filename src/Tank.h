//
//  Tank.hpp
//  CGXcode
//
//  Created by Philipp Lensing on 16.11.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#ifndef Tank_hpp
#define Tank_hpp

#include <stdio.h>
#include "model.h"

class Tank : public BaseModel
{
public:
	Tank();
	virtual ~Tank();
	bool loadModels(const char* ChassisFile, const char* CannonFile);
	void steer(float ForwardBackward, float LeftRight);
	void update(float dtime);
	virtual void draw(const BaseCamera& Cam);
protected:
	Model* pTankBot;
	Model* pTankTop;
	float time;
	float forwardBackward;
	float leftRight;
	Vector forward;
	Vector right;
};

#endif /* Tank_hpp */
