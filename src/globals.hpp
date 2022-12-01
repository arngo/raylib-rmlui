#pragma once
enum GameState {
    LOGO,
    MENU,
    GAME
};

enum MenuState {
    FADEIN,
    FADEOUT_START,
    FADEOUT,
    VISIBLE,
    INVISIBLE
};

typedef struct logoStateStruct {
	int state;
	int framesCounter;
	int lettersCount;
	int logoPositionX;
	int logoPositionY;
	int topSideRecWidth = 16;
	int leftSideRecHeight = 16;
	int bottomSideRecWidth = 16;
	int rightSideRecHeight = 16;
	float alpha = 1.0f;
} logoStateStruct;

extern GameState gameState;
extern MenuState menuState;
extern logoStateStruct logoState;
