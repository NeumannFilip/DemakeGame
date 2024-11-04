/*#include "DemakeTetris.h"
#include <iostream>

int main(int argc, char* argv[]) {
	using namespace std;
	const char* title = "DemakeTetris";
	DemakeTetris* Tetris = new DemakeTetris();
	if (Tetris->init(title)) {
		while (Tetris->isRunning()) {
			Tetris->HandleEvents();
			Tetris->Gameplay();
			Tetris->UpdateRenderer();
		}
	}
	else {
		cout << "Initialization Failed";
	}
	Tetris->clean();

	return 0;
}*/
