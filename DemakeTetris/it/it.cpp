// it.cpp : This file contains the 'main' function. Program execution begins and ends there.
//main.cpp

#include "DemakeTetris.h"
#include <iostream>

int main(int argc, char* argv[]) {
	using namespace std;
	//random number generation
	srand(time(0));
	const char* title = "DemakeTetris";
	//memory allocation that creates new instance of the DemakeTetris class
	DemakeTetris* Tetris = new DemakeTetris();

	if (Tetris->init(title)) 
	{
		while (Tetris->isRunning()) 
		{
			Tetris->setCurrentTime(SDL_GetTicks());

			Tetris->HandleEvents();

			if (!Tetris->GameOver()) {
				Tetris->Gameplay();
			}
			Tetris->UpdateRenderer();
		}
	}
	else {
		cout << "Initialization Failed";
	}
	Tetris->clean();

	return 0;
}
