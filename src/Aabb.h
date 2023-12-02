//
//  Aabb.hpp
//  CGXcode
//
//  Created by Philipp Lensing on 02.11.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#ifndef Aabb_hpp
#define Aabb_hpp

#include <stdio.h>
#include "vector.h"

// Axis aligned bounding box (AABB)
class AABB
{
public:
	int boundingX = 2;
	int boundingY = 2;
	int boundingZ = 2;
	Vector Min;
	Vector Max;
	AABB();
	AABB(const Vector& min, const Vector& max);
	AABB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ);
	Vector getBounding() { return bounding; };
	void setBounding(Vector bounding) { this->bounding = bounding; };
	Vector size() const;
protected:
	Vector bounding;
};



#endif /* Aabb_hpp */
