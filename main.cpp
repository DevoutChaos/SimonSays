#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

#define GLX_GLXEXT_LEGACY //Must be declared so that our local glxext.h is picked up, rather than the system one
#define _CRT_SECURE_NO_WARNINGS

#include <iomanip>
#include <locale>
#include <sstream>
#include <string>
#include <windows.h>
#include "windowOGL.h"
#include "GameConstants.h"
#include "cWNDManager.h"
#include "cInputMgr.h"
#include "cSoundMgr.h"
#include "cFontMgr.h"
#include "cSprite.h"
#include "asteroidsGame.h"
#include "cEnemy.h"

//Declarations
bool ylwRender = false;
bool redRender = false;
bool bluRender = false;
bool grnRender = false;
bool playersTurn = false;
bool bothFinished = false;
bool playerChecked = false;
bool readyCheck = true;
bool playTime = true;
bool playerWin = false;
bool playerLoss = false;
int correctCount = 0;
int curLoc = 0;
int counted = 0;
int score = 0;
int lives = 3;
string playerInput[10];

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR cmdLine,
	int cmdShow)
{

	//for debug
	AllocConsole();
	AttachConsole(GetCurrentProcessId());
	freopen("CON", "w", stdout);


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

	// load game sounds
	// Load Sound
	LPCSTR gameSounds[3] = { "Audio/Space_Coast.mp3", "Audio/shot007.wav", "Audio/explosion2.wav" };

	theSoundMgr->add("Theme", gameSounds[0]);
	theSoundMgr->add("Shot", gameSounds[1]);
	theSoundMgr->add("Explosion", gameSounds[2]);

	// load game fonts
	// Load Fonts
	LPCSTR gameFonts[2] = { "Fonts/digital-7.ttf", "Fonts/space age.ttf" };

	theFontMgr->addFont("SevenSeg", gameFonts[0], 24);
	theFontMgr->addFont("Space", gameFonts[1], 24);

	//Initialise the computer
	cEnemy computer;

	//Initialise the game background
	cTexture textureBkgd;
	textureBkgd.createTexture("Images\\BackGround_1280x720.png");
	cBkGround spriteBkgd;
	spriteBkgd.setSpritePos(glm::vec2(0.0f, 0.0f));
	spriteBkgd.setTexture(textureBkgd.getTexture());
	spriteBkgd.setTextureDimensions(textureBkgd.getTWidth(), textureBkgd.getTHeight());

	//Initialise the Pre-Game background
	cTexture textureRCBkgd;
	textureRCBkgd.createTexture("Images\\RCheckBackGround_1280x720.png");
	cBkGround spriteRCBkgd;
	spriteRCBkgd.setSpritePos(glm::vec2(0.0f, 0.0f));
	spriteRCBkgd.setTexture(textureRCBkgd.getTexture());
	spriteRCBkgd.setTextureDimensions(textureRCBkgd.getTWidth(), textureRCBkgd.getTHeight());

	//Initialise the victory background
	cTexture textureWinBkgd;
	textureWinBkgd.createTexture("Images\\WinBackGround_1280x720.png");
	cBkGround spriteWinBkgd;
	spriteWinBkgd.setSpritePos(glm::vec2(0.0f, 0.0f));
	spriteWinBkgd.setTexture(textureWinBkgd.getTexture());
	spriteWinBkgd.setTextureDimensions(textureWinBkgd.getTWidth(), textureWinBkgd.getTHeight());

	//Initialise the loss background
	cTexture textureLossBkgd;
	textureLossBkgd.createTexture("Images\\LossBackGround_1280x720.png");
	cBkGround spriteLossBkgd;
	spriteLossBkgd.setSpritePos(glm::vec2(0.0f, 0.0f));
	spriteLossBkgd.setTexture(textureLossBkgd.getTexture());
	spriteLossBkgd.setTextureDimensions(textureLossBkgd.getTWidth(), textureLossBkgd.getTHeight());

	//Monitor
	cTexture monitor;
	monitor.createTexture("Images\\Monitor.png");
	cRocket Monitor;
	Monitor.attachInputMgr(theInputMgr); // Attach the input manager to the sprite
	Monitor.setSpritePos(glm::vec2(517.5f, 225.0f));
	Monitor.setTexture(monitor.getTexture());
	Monitor.setTextureDimensions(monitor.getTWidth(), monitor.getTHeight());
	Monitor.setSpriteCentre();

	//Up
	cTexture blueTxt;
	blueTxt.createTexture("Images\\BlueSprite_245x245.png");
	cRocket BlueSprite;
	BlueSprite.attachInputMgr(theInputMgr); // Attach the input manager to the sprite
	BlueSprite.setSpritePos(glm::vec2(517.5f, 225.0f));
	BlueSprite.setTexture(blueTxt.getTexture());
	BlueSprite.setTextureDimensions(blueTxt.getTWidth(), blueTxt.getTHeight());
	BlueSprite.setSpriteCentre();

	//Down
	cTexture redTxt;
	redTxt.createTexture("Images\\RedSprite_245x245.png");
	cRocket RedSprite;
	RedSprite.attachInputMgr(theInputMgr); // Attach the input manager to the sprite
	RedSprite.setSpritePos(glm::vec2(517.5f, 475.0f));
	RedSprite.setTexture(redTxt.getTexture());
	RedSprite.setTextureDimensions(redTxt.getTWidth(), redTxt.getTHeight());
	RedSprite.setSpriteCentre();

	//Right
	cTexture greenTxt;
	greenTxt.createTexture("Images\\GreenSprite_245x245.png");
	cRocket GreenSprite;
	GreenSprite.attachInputMgr(theInputMgr); // Attach the input manager to the sprite
	GreenSprite.setSpritePos(glm::vec2(765.0f, 345.0f));
	GreenSprite.setTexture(greenTxt.getTexture());
	GreenSprite.setTextureDimensions(greenTxt.getTWidth(), greenTxt.getTHeight());
	GreenSprite.setSpriteCentre();

	//Left
	cTexture yellowTxt;
	yellowTxt.createTexture("Images\\YellowSprite_245x245.png");
	cRocket YellowSprite;
	YellowSprite.attachInputMgr(theInputMgr); // Attach the input manager to the sprite
	YellowSprite.setSpritePos(glm::vec2(270.0f, 345.0f));
	YellowSprite.setTexture(yellowTxt.getTexture());
	YellowSprite.setTextureDimensions(yellowTxt.getTWidth(), yellowTxt.getTHeight());
	YellowSprite.setSpriteCentre();

	//Up
	cTexture blueFlashTxt;
	blueFlashTxt.createTexture("Images\\BlueFlash_245x245.png");
	cRocket BlueFlash;
	BlueFlash.attachInputMgr(theInputMgr); // Attach the input manager to the sprite
	BlueFlash.setSpritePos(glm::vec2(517.5f, 225.0f));
	BlueFlash.setTexture(blueFlashTxt.getTexture());
	BlueFlash.setTextureDimensions(blueFlashTxt.getTWidth(), blueFlashTxt.getTHeight());
	BlueFlash.setSpriteCentre();

	//Down
	cTexture redFlashTxt;
	redFlashTxt.createTexture("Images\\RedFlash_245x245.png");
	cRocket RedFlash;
	RedFlash.attachInputMgr(theInputMgr); // Attach the input manager to the sprite
	RedFlash.setSpritePos(glm::vec2(517.5f, 475.0f));
	RedFlash.setTexture(redFlashTxt.getTexture());
	RedFlash.setTextureDimensions(redFlashTxt.getTWidth(), redFlashTxt.getTHeight());
	RedFlash.setSpriteCentre();

	//Right
	cTexture greenFlashTxt;
	greenFlashTxt.createTexture("Images\\GreenFlash_245x245.png");
	cRocket GreenFlash;
	GreenFlash.attachInputMgr(theInputMgr); // Attach the input manager to the sprite
	GreenFlash.setSpritePos(glm::vec2(765.0f, 345.0f));
	GreenFlash.setTexture(greenFlashTxt.getTexture());
	GreenFlash.setTextureDimensions(greenFlashTxt.getTWidth(), greenFlashTxt.getTHeight());
	GreenFlash.setSpriteCentre();

	//Left
	cTexture yellowFlashTxt;
	yellowFlashTxt.createTexture("Images\\YellowFlash_245x245.png");
	cRocket YellowFlash;
	YellowFlash.attachInputMgr(theInputMgr); // Attach the input manager to the Flash
	YellowFlash.setSpritePos(glm::vec2(270.0f, 345.0f));
	YellowFlash.setTexture(yellowFlashTxt.getTexture());
	YellowFlash.setTextureDimensions(yellowFlashTxt.getTWidth(), yellowFlashTxt.getTHeight());
	YellowFlash.setSpriteCentre();

	// Attach sound manager to rocket sprite
	BlueSprite.attachSoundMgr(theSoundMgr);
	GreenSprite.attachSoundMgr(theSoundMgr);
	RedSprite.attachSoundMgr(theSoundMgr);
	YellowSprite.attachSoundMgr(theSoundMgr);
	BlueFlash.attachSoundMgr(theSoundMgr);
	GreenFlash.attachSoundMgr(theSoundMgr);
	RedFlash.attachSoundMgr(theSoundMgr);
	YellowFlash.attachSoundMgr(theSoundMgr);

	//This is the mainloop, we render frames until isRunning returns false
	while (pgmWNDMgr->isWNDRunning())
	{
		pgmWNDMgr->processWNDEvents(); //Process any window events

		//We get the time that passed since the last frame
		float elapsedTime = pgmWNDMgr->getElapsedSeconds();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		/*if (!readyCheck)
		{
			spriteRCBkgd.render();
			Monitor.render();
			if (Monitor.readyToGo == true)
			{
				cout << "Hit ready check";
				readyCheck = true;
			}
		}*/
		if (readyCheck && playTime)
		{
			if (score == 10)
			{
				playerWin = true;
				playTime = false;
			}
			else if (lives == 0)
			{
				playerLoss = true;
				playTime = false;
			}

			spriteBkgd.render();
			GreenSprite.render();
			RedSprite.render();
			BlueSprite.render();
			YellowSprite.render();

			if (!playersTurn && playTime)
			{
				cout << "Turn No. " << computer.turnCount << "\n";
				cout << "Computer's turn" << "\n";
				computer.SetPattern();
				computer.DisplayPattern();
				playersTurn = true;
				cout << "Players Turn" << "\n";
			}
			else if (playersTurn && playTime)
			{
				if (GreenSprite.rightPressed == true)
				{
					GreenFlash.render();
					playerInput[curLoc] = "Green";
					counted++;
					curLoc++;
					GreenSprite.rightPressed = false;
				}

				if (RedSprite.downPressed == true)
				{
					RedFlash.render();
					playerInput[curLoc] = "Red";
					counted++;
					curLoc++;
					RedSprite.downPressed = false;
				}

				if (BlueSprite.upPressed == true)
				{
					BlueFlash.render();
					playerInput[curLoc] = "Blue";
					counted++;
					curLoc++;
					BlueSprite.upPressed = false;
				}

				if (YellowSprite.leftPressed == true)
				{
					YellowFlash.render();
					playerInput[curLoc] = "Yellow";
					counted++;
					curLoc++;
					YellowSprite.leftPressed = false;
				}
				if (counted == computer.turnCount)
				{
					for (int q = 0; q < computer.turnCount; q++)
					{
						cout << playerInput[q];
						if (computer.colourHolder[q] == playerInput[q])
						{
							cout << "Verified: " << q << "\n";
							correctCount++;
						}
					}
					cout << "Player Checked" << "\n";
					playerChecked = true;
				}

				if (playerChecked)
				{
					if (correctCount == computer.turnCount)
					{
						cout << "clearing";
						for (int c = 0; c < computer.turnCount; c++)
						{
							playerInput[c] = "";
						}
						computer.turnCount++;
						score++;
						correctCount = 0;
						counted = 0;
						curLoc = 0;
						playerChecked = false;
						playersTurn = false;
						cout << "\n" << "\n";
					}
					else if (correctCount != computer.turnCount)
					{
						cout << "clearing";
						for (int c = 0; c < computer.turnCount; c++)
						{
							playerInput[c] = "";
						}
						lives--;
						correctCount = 0;
						counted = 0;
						curLoc = 0;
						playerChecked = false;
						playersTurn = false;
						cout << "\n" << "\n";
					}
				}
			}

			GreenSprite.rightPressed = false;
			YellowSprite.leftPressed = false;
			BlueSprite.upPressed = false;
			RedSprite.downPressed = false;

			BlueSprite.update(elapsedTime);
			GreenSprite.update(elapsedTime);
			RedSprite.update(elapsedTime);
			YellowSprite.update(elapsedTime);
			BlueFlash.update(elapsedTime);
			GreenFlash.update(elapsedTime);
			RedFlash.update(elapsedTime);
			YellowFlash.update(elapsedTime);

			/*
			The folllowing line is based on code found at:
			http://www.cplusplus.com/articles/D9j2Nwbp/
			*/
			string String = static_cast<ostringstream*>(&(ostringstream() << score))->str();
			string baseString = "Score: ";
			baseString += String;

			string liveString = static_cast<ostringstream*>(&(ostringstream() << lives))->str();
			string livesString = "Lives: ";
			livesString += liveString;

			//Yeah, I'm too lazy to download another font, but hey, at least I changed the colour
			theFontMgr->getFont("SevenSeg")->printText(baseString.c_str(), FTPoint(20.0f, -20.0f, 0.0f));
			theFontMgr->getFont("SevenSeg")->printText(livesString.c_str(), FTPoint(200.0f, -20.0f, 0.0f));
		}

		if (playerLoss && !playTime)
		{
			spriteLossBkgd.render();
			Monitor.render();
			if (Monitor.restartGame)
			{

			}
			else if (Monitor.quitGame)
			{
				theOGLWnd.shutdown(); //Free any resources
				pgmWNDMgr->destroyWND(); //Destroy the program window
			}
		}
		if (playerWin && !playTime)
		{
			spriteWinBkgd.render();
			Monitor.render();
			if (Monitor.restartGame)
			{

			}
			else if (Monitor.quitGame)
			{
				theOGLWnd.shutdown(); //Free any resources
				pgmWNDMgr->destroyWND(); //Destroy the program window
			}
		}
		

		pgmWNDMgr->swapBuffers();
		theInputMgr->clearBuffers(theInputMgr->KEYS_DOWN_BUFFER | theInputMgr->KEYS_PRESSED_BUFFER);
	}

	theOGLWnd.shutdown(); //Free any resources
	pgmWNDMgr->destroyWND(); //Destroy the program window

	return 0; //Return success
}
