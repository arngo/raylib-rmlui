#include "raylib.h"
#include "raylib_logo.hpp"
#include "globals.hpp"

#include <cstdio>


int func()
{
	puts("test");
	return 0;
}

bool playLogoAnimation(struct logoStateStruct *stateStruct) {
	int logoPositionX = renderTargetWidth/2 - 128;
	int logoPositionY = renderTargetHeight/2 - 128;


	if (stateStruct->state == 0)                 // State 0: Small box blinking
	{
		stateStruct->framesCounter += 1;

		if (stateStruct->framesCounter == 120)
		{
			stateStruct->state = 1;
			stateStruct->framesCounter = 0;      // Reset counter... will be used later...
		}
	}
	else if (stateStruct->state == 1)            // State 1: Top and left bars growing
	{
		stateStruct->topSideRecWidth += 4;
		stateStruct->leftSideRecHeight += 4;

		if (stateStruct->topSideRecWidth == 256) stateStruct->state = 2;
	}
	else if (stateStruct->state == 2)            // State 2: Bottom and right bars growing
	{
		stateStruct->bottomSideRecWidth += 4;
		stateStruct->rightSideRecHeight += 4;

		if (stateStruct->bottomSideRecWidth == 256) stateStruct->state = 3;
	}
	else if (stateStruct->state == 3)            // State 3: Letters appearing (one by one)
	{
		stateStruct->framesCounter += 1;

		if (stateStruct->framesCounter/12)       // Every 12 frames, one more letter!
		{
			stateStruct->lettersCount += 1;
			stateStruct->framesCounter = 0;
		}

		if (stateStruct->lettersCount >= 10)     // When all letters have appeared, just fade out everything
		{
			stateStruct->alpha -= 0.02f;

			if (stateStruct->alpha <= 0.0f)
			{
				stateStruct->alpha = 0.0f;
				stateStruct->state = 4;
			}
		}
	}
	//----------------------------------------------------------------------------------

	// Draw
	//----------------------------------------------------------------------------------
	if (stateStruct->state == 0)
	{
		if ((stateStruct->framesCounter/15)%2) DrawRectangle(logoPositionX, logoPositionY, 16, 16, BLACK);
	}
	else if (stateStruct->state == 1)
	{
		DrawRectangle(logoPositionX, logoPositionY, stateStruct->topSideRecWidth, 16, BLACK);
		DrawRectangle(logoPositionX, logoPositionY, 16, stateStruct->leftSideRecHeight, BLACK);
	}
	else if (stateStruct->state == 2)
	{
		DrawRectangle(logoPositionX, logoPositionY, stateStruct->topSideRecWidth, 16, BLACK);
		DrawRectangle(logoPositionX, logoPositionY, 16, stateStruct->leftSideRecHeight, BLACK);

		DrawRectangle(logoPositionX + 240, logoPositionY, 16, stateStruct->rightSideRecHeight, BLACK);
		DrawRectangle(logoPositionX, logoPositionY + 240, stateStruct->bottomSideRecWidth, 16, BLACK);
	}
	else if (stateStruct->state == 3)
	{
		DrawRectangle(logoPositionX, logoPositionY, stateStruct->topSideRecWidth, 16, Fade(BLACK, stateStruct->alpha));
		DrawRectangle(logoPositionX, logoPositionY + 16, 16, stateStruct->leftSideRecHeight - 32, Fade(BLACK, stateStruct->alpha));

		DrawRectangle(logoPositionX + 240, logoPositionY + 16, 16, stateStruct->rightSideRecHeight - 32, Fade(BLACK, stateStruct->alpha));
		DrawRectangle(logoPositionX, logoPositionY + 240, stateStruct->bottomSideRecWidth, 16, Fade(BLACK, stateStruct->alpha));

		DrawRectangle(renderTargetWidth/2 - 112, renderTargetHeight/2 - 112, 224, 224, Fade(RAYWHITE, stateStruct->alpha));

		DrawText(TextSubtext("raylib", 0, stateStruct->lettersCount), renderTargetWidth/2 - 44, renderTargetHeight/2 + 48, 50, Fade(BLACK, stateStruct->alpha));
	}

	if(stateStruct->state == 4)
	{
		return true;
	}
	else
	{
		return false;
	}

}

