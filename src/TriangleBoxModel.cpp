//
//  TriangleBoxModel.cpp
//  CGXcode
//
//  Created by Philipp Lensing on 10.10.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#include "TriangleBoxModel.h"

TriangleBoxModel::TriangleBoxModel(float Width, float Height, float Depth)
{
	int boxSides = 6;
	float halfWidth = Width / 2.0f;
	float halfHeight = Height / 2.0f;
	float halfDepth = Depth / 2.0f;
	std::vector<Vector> side = { Vector(halfWidth, -halfHeight, -halfDepth), Vector(halfWidth, halfHeight, -halfDepth),
		Vector(halfWidth, halfHeight, halfDepth), Vector(halfWidth, -halfHeight, halfDepth) };
	std::vector<Vector> normal = { Vector(1, 0, 0), Vector(0, 0, -1), Vector(-1, 0, 0), Vector(0, 0, 1) };

	VB.begin();

	// Seiten
	for (int i = 0; i < 4; i++) {

		VB.addNormal(normal.at(i));
		VB.addTexcoord0(0, 1);
		VB.addVertex(side.at(0));

		VB.addTexcoord0(0, 0);
		VB.addVertex(side.at(1));

		VB.addTexcoord0(1, 0);
		VB.addVertex(side.at(2));

		VB.addTexcoord0(1, 1);
		VB.addVertex(side.at(3));

		Vector tmp[4] = { -side[2], -side[3], side[1], side[0] };
		std::copy(std::begin(tmp), std::end(tmp), std::begin(side));
	}

	side.clear();
	normal.clear();
	side = { Vector(-halfWidth, halfHeight, -halfDepth), Vector(-halfWidth, halfHeight, halfDepth),
		Vector(halfWidth, halfHeight, halfDepth), Vector(halfWidth, halfHeight, -halfDepth) };
	normal = { Vector(0, 1, 0), Vector(0, -1, 0) };

	// Oben & Unten
	for (int i = 0; i < 2; i++) {

		VB.addNormal(normal.at(i));
		VB.addTexcoord0(1, 0);
		VB.addVertex(side.at(0));

		VB.addTexcoord0(1, 1);
		VB.addVertex(side.at(1));

		VB.addTexcoord0(0, 1);
		VB.addVertex(side.at(2));

		VB.addTexcoord0(0, 0);
		VB.addVertex(side.at(3));

		Vector tmp[4] = { Vector(side[3].X, -side[3].Y, side[3].Z), Vector(side[2].X, -side[2].Y, side[2].Z), Vector(side[1].X, -side[1].Y, side[1].Z),
			Vector(side[0].X, -side[0].Y, side[0].Z) };
		std::copy(std::begin(tmp), std::end(tmp), std::begin(side));
	}

	VB.end();

	IB.begin();

	int b = 0;
	for (int i = 0; i < 6; i++) {
		IB.addIndex(0 + b);
		IB.addIndex(1 + b);
		IB.addIndex(2 + b);
		IB.addIndex(2 + b);
		IB.addIndex(3 + b);
		IB.addIndex(0 + b);
		b += 4;
	}

	IB.end();
}

void TriangleBoxModel::draw(const BaseCamera& Cam)
{
	BaseModel::draw(Cam);

	VB.activate();
	IB.activate();
	glDrawElements(GL_TRIANGLES, IB.indexCount(), IB.indexFormat(), 0);
	IB.deactivate();
	VB.deactivate();
}
