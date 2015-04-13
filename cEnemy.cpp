#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

#define GLX_GLXEXT_LEGACY //Must be declared so that our local glxext.h is picked up, rather than the system one
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "cEnemy.h"


void cEnemy::SetPattern()
{
	//Add colour to pattern
	i = turnCount-1;
	string colourPicker[] = { "Red", "Yellow", "Green", "Blue" };
	srand(time(NULL));
	tempNo = (rand() % 4);
	colourHolder[i] = colourPicker[tempNo];
}

void cEnemy::DisplayPattern()
{

	for (int x = 0; x < turnCount; x++)
	{
		if (colourHolder[x] == "Red")
		{
			cout << "Computer says " << "Red" << "\n";
		}
		else if (colourHolder[x] == "Yellow")
		{
			cout << "Computer says " << "Yellow" << "\n";
		}
		else if (colourHolder[x] == "Green")
		{
			cout << "Computer says " << "Green" << "\n";
		}
		else if (colourHolder[x] == "Blue")
		{
			cout << "Computer says " << "Blue" << "\n";

		}
	}
}