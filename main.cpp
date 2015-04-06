#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

#define GLX_GLXEXT_LEGACY //Must be declared so that our local glxext.h is picked up, rather than the system one


#include <windows.h>
#include "windowOGL.h"
#include "GameConstants.h"
#include "cWNDManager.h"
#include "cInputMgr.h"
#include "cSoundMgr.h"
#include "cFontMgr.h"
#include "cSprite.h"
#include "asteroidsGame.h"

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR cmdLine,
                   int cmdShow)
{

    //Set our window settings
    const int windowWidth = 1024;
    const int windowHeight = 768;
    const int windowBPP = 16;



    //This is our window
	static cWNDManager* pgmWNDMgr = cWNDManager::getInstance();

	// This is the input manager
	static cInputMgr* theInputMgr = cInputMgr::getInstance();

	// This is the sound manager
	static cSoundMgr* theSoundMgr = cSoundMgr::getInstance();

	// This is the Font manager
	static cFontMgr* theFontMgr = cFontMgr::getInstance();

	//The example OpenGL code
    windowOGL theOGLWnd;
	
    //Attach our the OpenGL window
	pgmWNDMgr->attachOGLWnd(&theOGLWnd);

	// Attach the keyboard manager
	pgmWNDMgr->attachInputMgr(theInputMgr);

    //Attempt to create the window
	if (!pgmWNDMgr->createWND(windowWidth, windowHeight, windowBPP))
    {
        //If it fails

        MessageBox(NULL, "Unable to create the OpenGL Window", "An error occurred", MB_ICONERROR | MB_OK);
		pgmWNDMgr->destroyWND(); //Reset the display and exit
        return 1;
    }

	if (!theOGLWnd.initOGL(windowWidth, windowHeight)) //Initialize our example
    {
        MessageBox(NULL, "Could not initialize the application", "An error occurred", MB_ICONERROR | MB_OK);
		pgmWNDMgr->destroyWND(); //Reset the display and exit
        return 1;
    }

	//Clear key buffers
	theInputMgr->clearBuffers(theInputMgr->KEYS_DOWN_BUFFER | theInputMgr->KEYS_PRESSED_BUFFER);

	/* initialize random seed: */
	srand((unsigned int)time(NULL));

	// Create vector array of textures
	LPCSTR texturesToUse[] = { "Images\\asteroid1.png", "Images\\asteroid2.png", "Images\\asteroid3.png", "Images\\asteroid4.png", "Images\\bullet.png" };
	for (int tCount = 0; tCount < 5; tCount++)
	{
		theGameTextures.push_back(new cTexture());
		theGameTextures[tCount]->createTexture(texturesToUse[tCount]);
	}

	// load game sounds
	// Load Sound
	LPCSTR gameSounds[3] = { "Audio/Space_Coast.mp3", "Audio/shot007.wav", "Audio/explosion2.wav" };

	theSoundMgr->add("Theme", gameSounds[0]);
	theSoundMgr->add("Shot", gameSounds[1]);
	theSoundMgr->add("Explosion", gameSounds[2]);

	// load game fontss
	// Load Fonts
	LPCSTR gameFonts[2] = { "Fonts/digital-7.ttf", "Fonts/space age.ttf" };

	theFontMgr->addFont("SevenSeg", gameFonts[0], 24);
	theFontMgr->addFont("Space", gameFonts[1], 24);

	// Create vector array of textures
	/*
	for (int astro = 0; astro < 5; astro++)
	{
		theAsteroids.push_back(new cAsteroid);
		theAsteroids[astro]->setSpritePos(glm::vec2(windowWidth / (rand() % 5 + 1), windowHeight / (rand() % 5 + 1)));
		theAsteroids[astro]->setSpriteTranslation(glm::vec2((rand() % 4 + 1), (rand() % 4 + 1)));
		int randAsteroid = rand() % 4;
		theAsteroids[astro]->setTexture(theGameTextures[randAsteroid]->getTexture());
		theAsteroids[astro]->setTextureDimensions(theGameTextures[randAsteroid]->getTWidth(), theGameTextures[randAsteroid]->getTHeight());
		theAsteroids[astro]->setSpriteCentre();
		theAsteroids[astro]->setAsteroidVelocity(glm::vec2(glm::vec2(0.0f, 0.0f)));
		theAsteroids[astro]->setActive(true);
		theAsteroids[astro]->setMdlRadius();
	}
	*/


	cTexture textureBkgd;
	textureBkgd.createTexture("Images\\BackGround_1280x720.png");
	cBkGround spriteBkgd;
	spriteBkgd.setSpritePos(glm::vec2(0.0f, 0.0f));
	spriteBkgd.setTexture(textureBkgd.getTexture());
	spriteBkgd.setTextureDimensions(textureBkgd.getTWidth(), textureBkgd.getTHeight());

	//Up
	cTexture blueTxt;
	blueTxt.createTexture("Images\\BlueSprite_245x245.png");
	cRocket BlueSprite;
	BlueSprite.attachInputMgr(theInputMgr); // Attach the input manager to the sprite
	BlueSprite.setSpritePos(glm::vec2(517.5f, 225.0f));
	BlueSprite.setTexture(blueTxt.getTexture());
	BlueSprite.setTextureDimensions(blueTxt.getTWidth(), blueTxt.getTHeight());
	BlueSprite.setSpriteCentre();
	//BlueSprite.setRocketVelocity(glm::vec2(0.0f, 0.0f));

	//Down
	cTexture redTxt;
	redTxt.createTexture("Images\\RedSprite_245x245.png");
	cRocket RedSprite;
	RedSprite.attachInputMgr(theInputMgr); // Attach the input manager to the sprite
	RedSprite.setSpritePos(glm::vec2(517.5f, 475.0f));
	RedSprite.setTexture(redTxt.getTexture());
	RedSprite.setTextureDimensions(redTxt.getTWidth(), redTxt.getTHeight());
	RedSprite.setSpriteCentre();
	//RedSprite.setRocketVelocity(glm::vec2(0.0f, 0.0f));

	//Right
	cTexture greenTxt;
	greenTxt.createTexture("Images\\GreenSprite_245x245.png");
	cRocket GreenSprite;
	GreenSprite.attachInputMgr(theInputMgr); // Attach the input manager to the sprite
	GreenSprite.setSpritePos(glm::vec2(765.0f, 345.0f));
	GreenSprite.setTexture(greenTxt.getTexture());
	GreenSprite.setTextureDimensions(greenTxt.getTWidth(), greenTxt.getTHeight());
	GreenSprite.setSpriteCentre();
	//GreenSprite.setRocketVelocity(glm::vec2(0.0f, 0.0f));

	//Left
	cTexture yellowTxt;
	yellowTxt.createTexture("Images\\YellowSprite_245x245.png");
	cRocket YellowSprite;
	YellowSprite.attachInputMgr(theInputMgr); // Attach the input manager to the sprite
	YellowSprite.setSpritePos(glm::vec2(270.0f, 345.0f));
	YellowSprite.setTexture(yellowTxt.getTexture());
	YellowSprite.setTextureDimensions(yellowTxt.getTWidth(), yellowTxt.getTHeight());
	YellowSprite.setSpriteCentre();
	//YellowSprite.setRocketVelocity(glm::vec2(0.0f, 0.0f));

	//Up
	cTexture blueFlashTxt;
	blueTxt.createTexture("Images\\BlueFlash_245x245.png");
	cRocket BlueFlash;
	BlueFlash.attachInputMgr(theInputMgr); // Attach the input manager to the sprite
	BlueFlash.setSpritePos(glm::vec2(517.5f, 225.0f));
	BlueFlash.setTexture(blueTxt.getTexture());
	BlueFlash.setTextureDimensions(blueTxt.getTWidth(), blueTxt.getTHeight());
	BlueFlash.setSpriteCentre();
	//BlueFlash.setRocketVelocity(glm::vec2(0.0f, 0.0f));

	//Down
	cTexture redFlashTxt;
	redTxt.createTexture("Images\\RedFlash_245x245.png");
	cRocket RedFlash;
	RedFlash.attachInputMgr(theInputMgr); // Attach the input manager to the sprite
	RedFlash.setSpritePos(glm::vec2(517.5f, 475.0f));
	RedFlash.setTexture(redTxt.getTexture());
	RedFlash.setTextureDimensions(redTxt.getTWidth(), redTxt.getTHeight());
	RedFlash.setSpriteCentre();
	//RedFlash.setRocketVelocity(glm::vec2(0.0f, 0.0f));

	//Right
	cTexture greenFlashTxt;
	greenTxt.createTexture("Images\\GreenFlash_245x245.png");
	cRocket GreenFlash;
	GreenFlash.attachInputMgr(theInputMgr); // Attach the input manager to the sprite
	GreenFlash.setSpritePos(glm::vec2(765.0f, 345.0f));
	GreenFlash.setTexture(greenTxt.getTexture());
	GreenFlash.setTextureDimensions(greenTxt.getTWidth(), greenTxt.getTHeight());
	GreenFlash.setSpriteCentre();
	//GreenFlash.setRocketVelocity(glm::vec2(0.0f, 0.0f));

	//Left
	cTexture yellowFlashTxt;
	yellowTxt.createTexture("Images\\YellowFlash_245x245.png");
	cRocket YellowFlash;
	YellowFlash.attachInputMgr(theInputMgr); // Attach the input manager to the Flash
	YellowFlash.setSpritePos(glm::vec2(270.0f, 345.0f));
	YellowFlash.setTexture(yellowTxt.getTexture());
	YellowFlash.setTextureDimensions(yellowTxt.getTWidth(), yellowTxt.getTHeight());
	YellowFlash.setSpriteCentre();
	//YellowFlash.setRocketVelocity(glm::vec2(0.0f, 0.0f));

	// Attach sound manager to rocket sprite
	BlueSprite.attachSoundMgr(theSoundMgr);
	GreenSprite.attachSoundMgr(theSoundMgr);
	RedSprite.attachSoundMgr(theSoundMgr);
	YellowSprite.attachSoundMgr(theSoundMgr);

    //This is the mainloop, we render frames until isRunning returns false
	while (pgmWNDMgr->isWNDRunning())
    {
		pgmWNDMgr->processWNDEvents(); //Process any window events

        //We get the time that passed since the last frame
		float elapsedTime = pgmWNDMgr->getElapsedSeconds();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		spriteBkgd.render();

		BlueSprite.update(elapsedTime);
		GreenSprite.update(elapsedTime);
		RedSprite.update(elapsedTime);
		YellowSprite.update(elapsedTime);

		vector<cAsteroid*>::iterator asteroidIterator = theAsteroids.begin();
		while (asteroidIterator != theAsteroids.end())
		{
			if ((*asteroidIterator)->isActive() == false)
			{
				asteroidIterator = theAsteroids.erase(asteroidIterator);
			}
			else
			{
				(*asteroidIterator)->update(elapsedTime);
				(*asteroidIterator)->render();
				++asteroidIterator;
			}
		}

		BlueSprite.render();
		GreenSprite.render();
		RedSprite.render();
		YellowSprite.render();
		//theFontMgr->getFont("Space")->printText("Asteriods", FTPoint(0.0f, -1.0f, 0.0f));

		pgmWNDMgr->swapBuffers();
		theInputMgr->clearBuffers(theInputMgr->KEYS_DOWN_BUFFER | theInputMgr->KEYS_PRESSED_BUFFER);
    }

	theOGLWnd.shutdown(); //Free any resources
	pgmWNDMgr->destroyWND(); //Destroy the program window

    return 0; //Return success
}
