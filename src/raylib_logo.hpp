
const int renderTargetWidth = 256;
const int renderTargetHeight = 256;
const int screenWidth = renderTargetWidth * 3;
const int screenHeight = renderTargetHeight * 3;

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

void playLogoAnimation(struct logoStateStruct *stateStruct);

int func();
