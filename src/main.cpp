/*******************************************************************************************
 *
 *   raylib [core] example - Basic window
 *
 *   Welcome to raylib!
 *
 *   To test examples, just press F6 and execute raylib_compile_execute script
 *   Note that compiled executable is placed in the same folder as .c file
 *
 *   You can find all basic examples on C:\raylib\raylib\examples folder or
 *   raylib official webpage: www.raylib.com
 *
 *   Enjoy using raylib. :)
 *
 *   This example has been created using raylib 1.0 (www.raylib.com)
 *   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
 *
 *   Copyright (c) 2013-2016 Ramon Santamaria (@raysan5)
 *
 ********************************************************************************************/
#define RMLUI_STATIC_LIB

#include "raylib.h"
#include "RmlUi/Core.h"
//#include <RmlUi/Debugger.h>
#include "RmlUi_Renderer_Raylib.h"
#include "RmlUi_Platform_Raylib.h"
#include "rlgl.h"

const int renderTargetWidth = 256;
const int renderTargetHeight = 256;
const int screenWidth = renderTargetWidth * 3;
const int screenHeight = renderTargetHeight * 3;
RenderTexture2D render_texture;

void playLogoAnimation() {
	SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    int logoPositionX = renderTargetWidth/2 - 128;
    int logoPositionY = renderTargetHeight/2 - 128;

    int framesCounter = 0;
    int lettersCount = 0;

    int topSideRecWidth = 16;
    int leftSideRecHeight = 16;

    int bottomSideRecWidth = 16;
    int rightSideRecHeight = 16;

    int state = 0;                  // Tracking animation states (State Machine)
    float alpha = 1.0f;             // Useful for fading

    while(state != 4) {
        if (state == 0)                 // State 0: Small box blinking
        {
            framesCounter++;

            if (framesCounter == 120)
            {
                state = 1;
                framesCounter = 0;      // Reset counter... will be used later...
            }
        }
        else if (state == 1)            // State 1: Top and left bars growing
        {
            topSideRecWidth += 4;
            leftSideRecHeight += 4;

            if (topSideRecWidth == 256) state = 2;
        }
        else if (state == 2)            // State 2: Bottom and right bars growing
        {
            bottomSideRecWidth += 4;
            rightSideRecHeight += 4;

            if (bottomSideRecWidth == 256) state = 3;
        }
        else if (state == 3)            // State 3: Letters appearing (one by one)
        {
            framesCounter++;

            if (framesCounter/12)       // Every 12 frames, one more letter!
            {
                lettersCount++;
                framesCounter = 0;
            }

            if (lettersCount >= 10)     // When all letters have appeared, just fade out everything
            {
                alpha -= 0.02f;

                if (alpha <= 0.0f)
                {
                    alpha = 0.0f;
                    state = 4;
                }
            }
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
            BeginTextureMode(render_texture);
            ClearBackground(RAYWHITE);

            if (state == 0)
            {
                if ((framesCounter/15)%2) DrawRectangle(logoPositionX, logoPositionY, 16, 16, BLACK);
            }
            else if (state == 1)
            {
                DrawRectangle(logoPositionX, logoPositionY, topSideRecWidth, 16, BLACK);
                DrawRectangle(logoPositionX, logoPositionY, 16, leftSideRecHeight, BLACK);
            }
            else if (state == 2)
            {
                DrawRectangle(logoPositionX, logoPositionY, topSideRecWidth, 16, BLACK);
                DrawRectangle(logoPositionX, logoPositionY, 16, leftSideRecHeight, BLACK);

                DrawRectangle(logoPositionX + 240, logoPositionY, 16, rightSideRecHeight, BLACK);
                DrawRectangle(logoPositionX, logoPositionY + 240, bottomSideRecWidth, 16, BLACK);
            }
            else if (state == 3)
            {
                DrawRectangle(logoPositionX, logoPositionY, topSideRecWidth, 16, Fade(BLACK, alpha));
                DrawRectangle(logoPositionX, logoPositionY + 16, 16, leftSideRecHeight - 32, Fade(BLACK, alpha));

                DrawRectangle(logoPositionX + 240, logoPositionY + 16, 16, rightSideRecHeight - 32, Fade(BLACK, alpha));
                DrawRectangle(logoPositionX, logoPositionY + 240, bottomSideRecWidth, 16, Fade(BLACK, alpha));

                DrawRectangle(renderTargetWidth/2 - 112, renderTargetHeight/2 - 112, 224, 224, Fade(RAYWHITE, alpha));

                DrawText(TextSubtext("raylib", 0, lettersCount), renderTargetWidth/2 - 44, renderTargetHeight/2 + 48, 50, Fade(BLACK, alpha));
            }
        EndTextureMode();
        BeginDrawing();
		DrawTexturePro(
				render_texture.texture,
				(Rectangle) { .x = 0, .y = 0, .width = renderTargetWidth, .height = -renderTargetHeight},
				(Rectangle) { .x = 0, .y = 0, .width = screenWidth, .height = screenHeight },
				(Vector2) { .x = 0, .y = 0 },
				0,
				WHITE
				);
        EndDrawing();
    }
}

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
	// Initialization
	//--------------------------------------------------------------------------------------

	InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

									//--------------------------------------------------------------------------------------
	render_texture = LoadRenderTexture(
			renderTargetWidth,
			renderTargetHeight
			);
	//auto system_interface = std::make_unique<SystemInterface_Raylib>();
	SystemInterface_Raylib system_interface;
	//auto render_interface = std::make_unique<RenderInterface_Raylib>();
	RenderInterface_Raylib render_interface;

	render_interface.SetViewport(screenWidth, screenHeight);

	Rml::SetSystemInterface(&system_interface);
	Rml::SetRenderInterface(&render_interface);

	Rml::Initialise();

	Rml::Context* context = Rml::CreateContext("default", Rml::Vector2i(renderTargetWidth, renderTargetHeight));
	
	if (!context)
		return 0;

	//Rml::Debugger::Initialise(context);

	Rml::LoadFontFace("assets/fonts/exo.regular.otf");
	//Rml::LoadFontFace("assets/fonts/Blobmoji.ttf");

	Rml::ElementDocument* document = context->LoadDocument("assets/rml/demo.rml");
	if (document)
		document->Show();
	else
		return 0;

    playLogoAnimation();
	SetTargetFPS(0);

	// Main game loop
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		// Update
		//----------------------------------------------------------------------------------
		// TODO: Update your variables here
		//----------------------------------------------------------------------------------

		document->ReloadStyleSheet();
		context->Update();
		// Draw
		//----------------------------------------------------------------------------------
		BeginTextureMode(render_texture);
		ClearBackground(BLANK);

		render_interface.BeginFrame();
		context->Render();
		render_interface.EndFrame();
		rlLoadIdentity();

		//DrawText("Congrats! You created your first window!", 190, 200, 20, ORANGE);

		DrawFPS(10, 10);

		EndTextureMode();
		// ---
		BeginDrawing();
		ClearBackground(RAYWHITE);
		DrawTexturePro(
				render_texture.texture,
				(Rectangle) { .x = 0, .y = 0, .width = renderTargetWidth, .height = -renderTargetHeight},
				(Rectangle) { .x = 0, .y = 0, .width = screenWidth, .height = screenHeight },
				(Vector2) { .x = 0, .y = 0 },
				0,
				WHITE
				);
		EndDrawing();
		//----------------------------------------------------------------------------------
	}

	// De-Initialization
	//--------------------------------------------------------------------------------------
	Rml::Shutdown();
	CloseWindow();        // Close window and OpenGL context
						  //--------------------------------------------------------------------------------------

	return 0;
}

