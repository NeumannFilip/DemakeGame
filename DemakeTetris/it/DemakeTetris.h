#pragma once
#ifndef DEMAKETETRIS_H_
#define DEMAKETETRIS_H_

//libraries that I use
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>

class DemakeTetris {

public:
	//Constructor 
	DemakeTetris();
	//Deconstructor
	~DemakeTetris();

	//accessible value of running to run a game
	bool isRunning() {

		return running;
	}
	//Setting time 
	void setCurrentTime(Uint32 time) {
		currentTime = time;
	}
	//Valid movement check
	bool isValid();
	//initialize the game
	bool init(const char* title);
	//Event = inputs to play the game
	void HandleEvents();
	//Core logic for blocking stacking and clearing blocks
	void Gameplay();
	//updating the scree
	void UpdateRenderer();
	//exit the game and clean
	void clean();
	//	Utility to set and adjust the position of structures in form of rectangles.
	void setPositionRect(SDL_Rect& rect, int x = 0, int y = 0, int w = BlockWidth, int h = BlockHeight);
	// Utility to set and adjust the position of structures in form of rectangles.
	void movePositionRect(SDL_Rect& rect, int x, int y);
	//in need of providing next Tetrimino ( as its called professionally)
	void nextTetrimino();
	//Checking if any part of new Tetrimino overlaps existing blocks
	bool GameOver();

private:
	//enum for declaring window size
	enum {ScreenWidth =420, ScreenHeight = 760};
	//enum for declaring the values of each block
	enum { BlockWidth = 36, BlockHeight = 36};
	//enum for the game grid
	enum {Lines = 20, Columns = 10};
	//pointers to SDL objects
	SDL_Window* window = NULL;
	SDL_Renderer* render = NULL;
	//pointers to Textures
	SDL_Texture* background = NULL;
	SDL_Texture* blocks = NULL;
	//Declaring the rectangle. ideally square but well, structre
	SDL_Rect sourceRect = { 0,0, BlockWidth, BlockHeight }, destR = { 0,0, BlockWidth, BlockHeight };

	//setting up running to false on default
	bool running = false;

	// Lines and Columns are dimensions 
	int field[Lines][Columns] = { 0 };
	// 2 dimensional array
	static const int figures[7][4];

	//pivot point to rotate blocks
	struct Point {
		int x, y;
	}tetrisBlocks[4], backup[4]; // storing coordinate
	// backup is for copying the tetrisBlocks array to backup array in case  I need to restore it if movement is invalid like trying to move outside of box/grid
	
	//color the block
	int color = 1;
	//horizontal block movement
	int horizontalMovementX = 0;
	//default flag of rotate to false
	bool rotate = false;
	//delay = time required for block to fall
	unsigned int delay = 250;
	//timer
	Uint32 startTime = 0, currentTime = 0;
	//font pointer
	TTF_Font* font;
	//score
	int score = 0;
	//lines cleard
	int linesCleared = 0;
	//textcolor
	SDL_Color textColor = { 255,255,255,255 };
};


#endif
