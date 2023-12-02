//
//  LineBoxModel.cpp
//  CGXcode
//
//  Created by Philipp Lensing on 10.10.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#include "LineBoxModel.h"

LineBoxModel::LineBoxModel(float Width, float Height, float Depth)
{
	VB.begin();

	float halfWidth = Width / 2.0f;
	float halfDepth = Depth / 2.0f;

	// Bodenkanten
	VB.addVertex(halfWidth, 0, -halfDepth); // V0
	VB.addVertex(-halfWidth, 0, -halfDepth); // V1

	VB.addVertex(-halfWidth, 0, -halfDepth); // V1
	VB.addVertex(-halfWidth, 0, halfDepth); // V2

	VB.addVertex(-halfWidth, 0, halfDepth); // V2
	VB.addVertex(halfWidth, 0, halfDepth); // V3

	VB.addVertex(halfWidth, 0, halfDepth); // V3
	VB.addVertex(halfWidth, 0, -halfDepth); // V0

	// Obere Kanten
	VB.addVertex(halfWidth, Height, -halfDepth); // V4
	VB.addVertex(-halfWidth, Height, -halfDepth); // V5

	VB.addVertex(-halfWidth, Height, -halfDepth); // V5
	VB.addVertex(-halfWidth, Height, halfDepth); // V6

	VB.addVertex(-halfWidth, Height, halfDepth); // V6
	VB.addVertex(halfWidth, Height, halfDepth); // V7

	VB.addVertex(halfWidth, Height, halfDepth); // V7
	VB.addVertex(halfWidth, Height, -halfDepth); // V4

	// Seitenkanten
	VB.addVertex(halfWidth, 0, -halfDepth); // V0
	VB.addVertex(halfWidth, Height, -halfDepth); // V4

	VB.addVertex(-halfWidth, 0, -halfDepth); // V1
	VB.addVertex(-halfWidth, Height, -halfDepth); // V5

	VB.addVertex(-halfWidth, 0, halfDepth); // V2
	VB.addVertex(-halfWidth, Height, halfDepth); // V6

	VB.addVertex(halfWidth, 0, halfDepth); // V3
	VB.addVertex(halfWidth, Height, halfDepth); // V7

	VB.end();
}

void LineBoxModel::draw(const BaseCamera& Cam)
{
	BaseModel::draw(Cam);

	VB.activate();

	// Verbinden mit GL_LINES als Zeichenprimitiva
	glDrawArrays(GL_LINES, 0, VB.vertexCount());

	VB.deactivate();
}
