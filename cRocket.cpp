/*
=================
cRocket.cpp
- Header file for class definition - IMPLEMENTATION
=================
*/
#include "cRocket.h"


void cRocket::render()
{
	glPushMatrix();

	glTranslatef(spritePos2D.x, spritePos2D.y, 0.0f);
	glRotatef(spriteRotation, 0.0f, 0.0f, 1.0f);
	glScalef(spriteScaling.x, spriteScaling.y, 1.0f);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, GLTextureID); // Binding of GLtexture name 

	glBegin(GL_QUADS);
	glColor3f(255.0f, 255.0f, 255.0f);
	glTexCoord2f(spriteTexCoordData[0].x, spriteTexCoordData[0].y);
	glVertex2f(-(textureWidth / 2), -(textureHeight / 2));
	glTexCoord2f(spriteTexCoordData[1].x, spriteTexCoordData[1].y);
	glVertex2f((textureWidth / 2), -(textureHeight / 2));
	glTexCoord2f(spriteTexCoordData[2].x, spriteTexCoordData[2].y);
	glVertex2f((textureWidth / 2), (textureHeight / 2));
	glTexCoord2f(spriteTexCoordData[3].x, spriteTexCoordData[3].y);
	glVertex2f(-(textureWidth / 2), (textureHeight / 2));

	glEnd();
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}
/*
=================================================================
Update the sprite position
=================================================================
*/

void cRocket::update(float deltaTime)
{
	m_SoundMgr->getSnd("Theme")->playAudio(AL_TRUE);
	if (m_InputMgr->isKeyDown(VK_RIGHT))
	{
		cout << "Do you see me?";
		rightPressed = true;
	}
	if (m_InputMgr->isKeyDown(VK_LEFT))
	{
		leftPressed = true;
	}
	if (m_InputMgr->isKeyDown(VK_UP))
	{
		upPressed = true;
	}
	if (m_InputMgr->isKeyDown(VK_DOWN))
	{
		downPressed = true;
	}	
	if (m_InputMgr->isKeyDown(int('R')))
	{
		restartGame = true;
	}
	if (m_InputMgr->isKeyDown(int('Q')))
	{
		quitGame = true;
	}
	if (m_InputMgr->isKeyDown(int('B')))
	{
		cout << "Pressed";
		readyToGo = true;
	}
}