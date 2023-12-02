//
//  Tank.cpp
//  CGXcode
//
//  Created by Philipp Lensing on 16.11.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#include "Tank.h"

Tank::Tank()
{
	// TODO: Add your code
}

Tank::~Tank()
{
	delete pTankTop;
	delete pTankBot;
}

bool Tank::loadModels(const char* ChassisFile, const char* CannonFile)
{
	pTankTop = new Model(CannonFile, false);
	pTankTop->shader(pShader, DeleteShader);
	pTankBot = new Model(ChassisFile, false);
	pTankBot->shader(pShader, DeleteShader);
	return true;
}

void Tank::steer(float ForwardBackward, float LeftRight)
{
	forwardBackward = ForwardBackward;
	leftRight = LeftRight;
}

void Tank::update(float dTime)
{
	time += dTime;

	Matrix translationMatrix, rotationMatrix;

	//  Translation Bottom
	translationMatrix.translation(forwardBackward * dTime * 2, 0, 0); // 2 nur ein Geschwindigkeitsfaktor
	pTankBot->transform(pTankBot->transform() * translationMatrix); // aktuelle Position * veränderte Position

	// Translation Top (in Abhängigkeit von Bottom) <- ist an der Stelle aber irrelevant, da bzgl. des aims eh eine weitere translation gemacht wird
	translationMatrix.translation(pTankBot->transform().translation()); // Positionsvektor des Bottoms holen und Haubitze dorthin platzieren
	pTankTop->transform(translationMatrix);

	// Rotation Bottom
	rotationMatrix.rotationY(leftRight * dTime * 2); // 2 nur ein Geschwindigkeitsfaktor
	pTankBot->transform(pTankBot->transform() * rotationMatrix);

	// forwardBackward und leftRight zurücksetzen
	forwardBackward = 0;
	leftRight = 0;
}

void Tank::draw(const BaseCamera& Cam)
{
	pTankBot->draw(Cam);
	pTankTop->draw(Cam);
}
