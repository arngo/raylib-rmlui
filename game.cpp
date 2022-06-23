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


#include <raylib.h>
#include <RmlUi/Core.h>
//#include <RmlUi/Debugger.h>
#include "RmlUi_Renderer_Raylib.h"
#include "RmlUi_Platform_Raylib.h"

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
	// Initialization
	//--------------------------------------------------------------------------------------
	const int screenWidth = 800;
	const int screenHeight = 450;

	InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

	SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
									//--------------------------------------------------------------------------------------
	//auto system_interface = std::make_unique<SystemInterface_Raylib>();
	SystemInterface_Raylib system_interface;
	//auto render_interface = std::make_unique<RenderInterface_Raylib>();
	RenderInterface_Raylib render_interface;

	render_interface.SetViewport(screenWidth, screenHeight);

	Rml::SetSystemInterface(&system_interface);
	Rml::SetRenderInterface(&render_interface);

	Rml::Initialise();

	Rml::Context* context = Rml::CreateContext("default", Rml::Vector2i(screenWidth, screenHeight));
	
	if (!context)
		return 0;

	//Rml::Debugger::Initialise(context);

	Rml::LoadFontFace("exo.regular.otf");
	Rml::LoadFontFace("Blobmoji.ttf");

	Rml::ElementDocument* document = context->LoadDocument("demo.rml");
	if (document)
		document->Show();
	else
		return 0;

	// Main game loop
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		// Update
		//----------------------------------------------------------------------------------
		// TODO: Update your variables here
		//----------------------------------------------------------------------------------

		context->Update();
		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();

		ClearBackground(RAYWHITE);

		render_interface.BeginFrame();

		context->Render();

		DrawText("Congrats! You created your first window!", 190, 200, 20, ORANGE);

		render_interface.EndFrame();
		EndDrawing();
		//----------------------------------------------------------------------------------
	}

	// De-Initialization
	//--------------------------------------------------------------------------------------
	CloseWindow();        // Close window and OpenGL context
						  //--------------------------------------------------------------------------------------

	return 0;
}

