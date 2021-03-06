/*
=================
cRocket.h
- Header file for class definition - SPECIFICATION
- Header file for the Rocket class which is a child of cSprite class
=================
*/
#ifndef _CROCKET_H
#define _CROCKET_H
#include "cSprite.h"
#include "asteroidsGame.h"

class cRocket : public cSprite
{
private:
	//glm::vec2 rocketVelocity = glm::vec2(0.0f, 0.0f);

public:
	void render();		// Default render function
	void update(float deltaTime);		// Rocket update method
	bool leftPressed;
	bool rightPressed;
	bool upPressed;
	bool downPressed;
	bool restartGame;
	bool quitGame;
	bool readyToGo;
};
#endif