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
//Flash render bools
bool ylwRender = false;
bool redRender = false;
bool bluRender = false;
bool grnRender = false;
//bools to manage the given "turns" and checking
bool playersTurn = false;
bool bothFinished = false;
bool playerChecked = false;
//bools which should be used to change "scenes"
//readyCheck should be false
bool readyCheck = true;
bool playTime = true;
bool playerWin = false;
bool playerLoss = false;
//ints used to monitor player inputs, and accuracy compared to the AI
int correctCount = 0;
int curLoc = 0;
int counted = 0;
//ints used to track player score and lives
int score = 0;
int lives = 3;
//Array to hold all of the players inputs
string playerInput[10];

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR cmdLine,
	int cmdShow)
{

	//Code to add the console window
	//Given that the flashes don't work, this is the only way for the game to actually work now
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
	LPCSTR gameSounds[3] = { "Audio/tetris-gameboy-02.mp3", "Audio/shot007.wav", "Audio/explosion2.wav" };

	/*
	Theme music obtained from:
	http://downloads.khinsider.com/game-soundtracks/album/tetris-gameboy-rip-/tetris-gameboy-02.mp3
	Insert legal jargon about not owning copyright etc here
	*/
	theSoundMgr->add("Theme", gameSounds[0]);
	theSoundMgr->add("Shot", gameSounds[1]);
	theSoundMgr->add("Explosion", gameSounds[2]);

	// load game fonts
	// Load Fonts
	LPCSTR gameFonts[2] = { "Fonts/digital-7.ttf", "Fonts/space age.ttf" };

	//Fonts are still the ones included in the lab8 folders.
	//SevenSeg fits in my opinion
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
		
		/*
		This code should be included, but given that the monitor doesn't accept input, it had to be removed
		*/
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

		//This is the main game, runs only after the player has selected to start the game
		if (readyCheck && playTime)
		{
			//Runs when the player wins
			if (score == 10)
			{
				playerWin = true;
				playTime = false;
			}
			//Runs when the player loses
			else if (lives == 0)
			{
				playerLoss = true;
				playTime = false;
			}

			//Renders the main background and sprites
			spriteBkgd.render();
			GreenSprite.render();
			RedSprite.render();
			BlueSprite.render();
			YellowSprite.render();

			//Runs for the AI turn
			if (!playersTurn && playTime)
			{
				//Gives the turn No. and also generates and outputs the AIs pattern, before informing the player that it is their turn
				cout << "Turn No. " << computer.turnCount << "\n";
				cout << "Computer's turn" << "\n";
				computer.SetPattern();
				computer.DisplayPattern();
				playersTurn = true;
				cout << "Players Turn" << "\n";
			}
			//Runs for the player turn
			else if (playersTurn && playTime)
			{
				//Series of if statements to check whether or not the player has pressed the arrow keys
				if (GreenSprite.rightPressed == true)
				{
					//Should flash the appropriate texture on key press (is buggy and only sometimes flashes)
					GreenFlash.render();
					//Adds input to the players input array
					playerInput[curLoc] = "Green";
					//Increases check values
					counted++;
					curLoc++;
					//Sets this boolean to false so that it should only run once
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
				//When the player has made the same number of inputs as the AI
				if (counted == computer.turnCount)
				{
					//Loop through all of the inputs
					for (int q = 0; q < computer.turnCount; q++)
					{
						//Reiterate the player outputs
						cout << playerInput[q];
						//If the player input matches the AI input for that index
						if (computer.colourHolder[q] == playerInput[q])
						{
							//Notify the player and increase the counter
							cout << "Verified: " << q << "\n";
							correctCount++;
						}
					}
					//After the loop has completed switch a boolean, and inform the player
					cout << "Player Checked" << "\n";
					playerChecked = true;
				}

				//After checking the player
				if (playerChecked)
				{
					//If everything was correct
					if (correctCount == computer.turnCount)
					{
						//Inform the player that their array is being cleared
						cout << "clearing";
						//Clear the players array
						for (int c = 0; c < computer.turnCount; c++)
						{
							playerInput[c] = "";
						}
						//Increase the difficulty by one
						computer.turnCount++;
						//Increment score
						score++;
						//Reset all values
						correctCount = 0;
						counted = 0;
						curLoc = 0;
						playerChecked = false;
						playersTurn = false;
						//Move down a couple of lines to separate the turns
						cout << "\n" << "\n";
					}
					else if (correctCount != computer.turnCount)
					{
						//Inform the player that their array is being cleared
						cout << "clearing";
						//Clear the players array
						for (int c = 0; c < computer.turnCount; c++)
						{
							playerInput[c] = "";
						}
						//Decrement lives
						lives--;
						//Reset all values
						correctCount = 0;
						counted = 0;
						curLoc = 0;
						playerChecked = false;
						playersTurn = false;
						//Move down a couple of lines to separate the turns
						cout << "\n" << "\n";
					}
				}
			}

			//Ensure that all key presses are reset
			GreenSprite.rightPressed = false;
			YellowSprite.leftPressed = false;
			BlueSprite.upPressed = false;
			RedSprite.downPressed = false;

			//Run update
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
			Ensures that the score and lives are both output on the game window
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

		//When the player has run out of lives
		if (playerLoss && !playTime)
		{
			//Change "scene"
			spriteLossBkgd.render();
			Monitor.render();
			//Reset the game
			if (Monitor.restartGame)
			{
				//Untested for obvious reasons, so I may have missed something
				computer.turnCount = 1;
				lives = 3;
				score = 0;
				bool playersTurn = false;
				bool bothFinished = false;
				bool playerChecked = false;
				bool readyCheck = true;
				bool playTime = true;
				bool playerWin = false;
				bool playerLoss = false;
			}
			//Close the game
			else if (Monitor.quitGame)
			{
				theOGLWnd.shutdown(); //Free any resources
				pgmWNDMgr->destroyWND(); //Destroy the program window
			}
		}
		//When the player reaches 1- points
		if (playerWin && !playTime)
		{
			//Change "scene"
			spriteWinBkgd.render();
			Monitor.render();
			//Reset the game
			if (Monitor.restartGame)
			{
				//Untested for obvious reasons, so I may have missed something
				computer.turnCount = 1;
				lives = 3;
				score = 0;
				bool playersTurn = false;
				bool bothFinished = false;
				bool playerChecked = false;
				bool readyCheck = true;
				bool playTime = true;
				bool playerWin = false;
				bool playerLoss = false;
			}
			//Close the game
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
