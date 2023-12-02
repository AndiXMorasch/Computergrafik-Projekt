//
//  BaseModel.hpp
//  ogl4
//
//  Created by Philipp Lensing on 19.09.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#ifndef BaseModel_hpp
#define BaseModel_hpp

#include <stdio.h>
#include "camera.h"
#include "matrix.h"
#include "baseshader.h"

class BaseModel
{
public:
	BaseModel();
	virtual ~BaseModel();
	virtual void draw(const BaseCamera& Cam);
	const Matrix& transform() const { return Transform; }
	void transform(const Matrix& m) { Transform = m; }
	virtual Vector getPosition() { return transform().translation(); };
	bool isTeleported() { return teleported; };
	void setTeleported(bool teleported) { this->teleported = teleported; };
	virtual void shader(BaseShader* shader, bool deleteOnDestruction = false);
	virtual BaseShader* shader() const { return pShader; }
	float radius = 0.0f;
	float getRadius() { return radius; };
	void setRadius(float newRadius) { this->radius = newRadius; };
protected:
	Matrix Transform;
	BaseShader* pShader;
	bool DeleteShader;
	bool teleported = false;
};


#endif /* BaseModel_hpp */
