#include <iostream>
#include "DemakeTetris.h"
/*
	General information:
	arrays listed in header are:
	tetrisBlocks that contain the tetriminos
	backup that contain the tetrisBlocks information and if something illegal happens during the game I can restore the positions.
*/

bool DemakeTetris::isValid()
{
	//iterating over tetrisBlocks that contains the current position of 4 blocks that make the tetrimino
	for (int i = 0; i < 4; i++) 
	{
		//checking if conditions are met for validaty of block position
		if (tetrisBlocks[i].x < 0 || tetrisBlocks[i].x >= Columns || tetrisBlocks[i].y >= Lines)
		{
			return false;
		}
		//else if its out of bounds do the same 
		else if (field[tetrisBlocks[i].y][tetrisBlocks[i].x])
		{

			return false;
		}
	}
	return true;
}
/// <summary>
/// Create Window and renderer
/// </summary>
/// <param name="title"></param>
/// <returns></returns>
bool DemakeTetris::init(const char* title)
{
	//initializing everything
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) 
	{
		SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
		window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, ScreenWidth, ScreenHeight, SDL_WINDOW_SHOWN);
		
		if (window != NULL) 
		{
			
			render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
			
			if (render != NULL)
			{
				
				SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
				
				int imgFlags = IMG_INIT_PNG;
				int initted = IMG_Init(imgFlags);
				if ((initted & imgFlags) != imgFlags) {
					std::cout << "Initialization Failed learn to get png\n" << "IMG_INIT Error" << IMG_GetError() << std::endl;
				}

				//find background
				SDL_Surface* loadSurface = IMG_Load("Images/tetrisbackground.png");
				background = SDL_CreateTextureFromSurface(render, loadSurface);
				SDL_FreeSurface(loadSurface);
				
				//find blocks
				loadSurface = IMG_Load("Images/Tetrisblocks.png");
				blocks = SDL_CreateTextureFromSurface(render, loadSurface);
				SDL_FreeSurface(loadSurface);
				nextTetrimino();

				// Attempt at using SLD_ttf to print score and time on the screen. I encountered too many issues with loading font
				/*if (TTF_Init() == -1) {
					std::cout << "TTF_Init: " << TTF_GetError() << std::endl;
					SDL_Quit();
					return false;
				}

				// Load font
				font = TTF_OpenFont("Debug/Font.ttf", 24); 
				if (!font) {
					std::cout << "Failed to load font: " << TTF_GetError() << std::endl;
					TTF_Quit();
					SDL_Quit();
					return false;
				}*/
				
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
	running = true;
	return true;
}


DemakeTetris::DemakeTetris()
{
}
DemakeTetris::~DemakeTetris() {
}

void DemakeTetris::HandleEvents()
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			running = false;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_UP:
				rotate = true;
				break;
			case SDLK_LEFT:
				horizontalMovementX = -1;
				break;
			case SDLK_RIGHT:
				horizontalMovementX = 1;
				break;
			case SDLK_DOWN:
				delay = 50;
				break;
			}
		default:
			break;
		}
	}
	//handle keyboard input; so if we hold down arrow it will increase speed of block falling down
	const Uint8* state = SDL_GetKeyboardState(NULL);
	//looking for down arrow
	if (state[SDL_SCANCODE_DOWN])
	{
		delay = 50;
	}
}

/// <summary>
/// Core gameplay logic
/// Moving, rotating, "falling", clearing full rows and valid checks
/// </summary>
void DemakeTetris::Gameplay()
{
	// backup
	// copying the items array to backup array 
	// I do it to restore the state if movement is invalid like trying to move outside of box/grid
	for (int i = 0; i < 4; i++) {
		backup[i] = tetrisBlocks[i];
	}

	// moving blocks
	if (horizontalMovementX)
	{
		for (int i = 0; i < 4; i++)
		{
			tetrisBlocks[i].x += horizontalMovementX;
		}
		if (!isValid()) {
			for (int i = 0; i < 4; i++) {
				tetrisBlocks[i] = backup[i];
			}
		}
	}

	//rotate blocks
	if (rotate) {
		// pivot point 2nd square of the block
		Point p = tetrisBlocks[2];

		for (int i = 0; i < 4; i++)
		{
			//calculating x and y relative position to the pivot point
			//In 2D space a 90-degree clockwise rotation is given by (newX, newY) = (-oldY, oldX). 
			int x = tetrisBlocks[i].y - p.y;
			int y = tetrisBlocks[i].x - p.x;
			tetrisBlocks[i].x = p.x - x;
			tetrisBlocks[i].y = p.y + y;
		}
		if (!isValid()) {
			for (int i = 0; i < 4; i++)
			{
				tetrisBlocks[i] = backup[i];
			}
		}
	}

	// tick
	if (currentTime - startTime > delay) 
	{
		//backup block position
		for (int i = 0; i < 4; i++)
		{
			backup[i] = tetrisBlocks[i];
		}
		for (int i = 0; i < 4; i++) 
		{
			tetrisBlocks[i].y++;
		}
		if (!isValid()) 
		{
			for (int i = 0; i < 4; i++)
			{
				field[backup[i].y][backup[i].x] = color;
			}
			nextTetrimino();
		}
		startTime = currentTime;
	}

	//Lines check for deleting rows
	for (int i = Lines - 1; i >= 0; i--)
	{
		int count = 0;
		
		for (int j = 0; j < Columns; j++)
		{
			if (field[i][j]) count++;
		}

		//if line is full I remove it
		if (count == Columns) {
			// Shift down all lines above
			for (int k = i; k > 0; k--)
			{
				for (int j = 0; j < Columns; j++) 
				{
					field[k][j] = field[k - 1][j];
				}
			}
			for (int j = 0; j < Columns; j++)
			{
				field[0][j] = 0;
			}

			i++;
			linesCleared++;
			score += 100;
		}
	}
	horizontalMovementX = 0;
	rotate = false;
	delay = 200;
}

/// <summary>
/// Rendering the game
/// </summary>
void DemakeTetris::UpdateRenderer()
{
	SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
	SDL_RenderClear(render);
	SDL_RenderCopy(render, background, NULL, NULL);

	//Nesting loop for each cell, that represents tetris board/grid
	for (int i = 0; i < Lines; i++)
	{
		for (int j = 0; j < Columns; j++) 
		{
			if (field[i][j]) 
			{
				setPositionRect(sourceRect, field[i][j] * BlockWidth);
				setPositionRect(destR, j * BlockWidth, i * BlockHeight);
				movePositionRect(destR, BlockWidth, ScreenHeight - (Lines + 1) * BlockHeight);
				SDL_RenderCopy(render, blocks, &sourceRect, &destR);
			}
		}
	}
	//Rendering moving objects, actually the one that is in motion
	//Set the rectangles and draw blocks based on the tetrisBlocks array position
	for (int i = 0; i < 4; i++)
	{
		setPositionRect(sourceRect, color * BlockWidth);
		setPositionRect(destR, tetrisBlocks[i].x * BlockWidth, tetrisBlocks[i].y * BlockHeight);
		movePositionRect(destR, BlockWidth, ScreenHeight - (Lines + 1) * BlockHeight);
		SDL_RenderCopy(render, blocks, &sourceRect, &destR);
	}

	// Render score text
	/*std::string scoreText = "Score: " + std::to_string(score);
	SDL_Surface* scoreSurface = TTF_RenderText_Solid(font, scoreText.c_str(), textColor);
	SDL_Texture* scoreTexture = SDL_CreateTextureFromSurface(render, scoreSurface);
	SDL_Rect scoreRect = { 10, 10, scoreSurface->w, scoreSurface->h };
	SDL_RenderCopy(render, scoreTexture, NULL, &scoreRect);
	SDL_FreeSurface(scoreSurface);
	SDL_DestroyTexture(scoreTexture);


	//Render the text
	std::string linesText = "Lines: " + std::to_string(linesCleared);
	SDL_Surface* linesSurface = TTF_RenderText_Solid(font, linesText.c_str(), textColor);
	SDL_Texture* linesTexture = SDL_CreateTextureFromSurface(render, linesSurface);
	SDL_Rect linesRect = { 10, 50, linesSurface->w, linesSurface->h }; // Adjust position as needed
	SDL_RenderCopy(render, linesTexture, NULL, &linesRect);
	SDL_FreeSurface(linesSurface);
	SDL_DestroyTexture(linesTexture);*/
	
	//update the screen
	SDL_RenderPresent(render);
}

//Blocks
// for this I think 4x4 box is good 
// so it might look like this 
/*
*  0  1  2  3
*  4  5  6  7
* 
* 
*/

/// <summary>
/// Standard tetris block shapes within 4x4 grid
/// each row in this Figures array corresponds to different tetris block
/// so for the example:
/// 0,1,2,3 Straight block occupies a straight line in top row of the grid. etc.
/// </summary>
const int DemakeTetris::figures[7][4] =
{
	0,1,2,3,	// Straight Block
	0,4,5,6,	// J-Shaped Block
	2,6,5,4,	// L-Shaped Block
	1,2,5,6,	// Square Shaped Block
	2,1,5,4,	// S-Shapede Block
	1,4,5,6,	// T-Shaped Block
	0,1,5,6,	// Z-Shaped Block
};

//clean up and quit
void DemakeTetris::clean()
{
	SDL_DestroyTexture(blocks);
	SDL_DestroyTexture(background);
	SDL_DestroyRenderer(render);
	//TTF_CloseFont(font);
	//TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

void DemakeTetris::setPositionRect(SDL_Rect& rect, int x, int y, int w, int h)
{
	rect = { x,y,w,h };
}


void DemakeTetris::movePositionRect(SDL_Rect& rect, int x, int y)
{
	rect.x += x;
	rect.y += y;
}

void DemakeTetris::nextTetrimino()
{
	//assign color to blocks
	color = 1 + rand() % 7;
	// select one of the 7 available figures
	int n = rand() % 7;
	for (int i = 0; i < 4; i++)
	{
		//set coordinates
		tetrisBlocks[i].x = figures[n][i] % 4;
		tetrisBlocks[i].y = int(figures[n][i] / 4);
	}
}


/// <summary>
/// Checking if any part of new Tetrimino overlaps existing blocks
/// </summary>
/// <returns></returns>
bool DemakeTetris::GameOver()
{
	for (int i = 0; i < 4; i++) 
	{
		if (field[tetrisBlocks[i].y][tetrisBlocks[i].x])
		{
			return true;
		}
	}
	return false;
}
