#ifndef _CENEMY_H
#define _CENEMY_H
#include <iostream>
using namespace std;

class cEnemy
{
private:
	string colourPicker[4];
	int tempNo;
	int counter;

public:
	void SetPattern();
	void DisplayPattern();
	int turnCount = 1;
	string colourHolder[10];
};

#endif