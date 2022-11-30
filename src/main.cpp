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

#include "raylib_logo.hpp"

RenderTexture2D render_texture;

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
	// Initialization
	//--------------------------------------------------------------------------------------

	InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

	logoStateStruct logoState = { 0 };

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


	SetTargetFPS(60);
	// Main game loop
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		// Update
		//----------------------------------------------------------------------------------
		// TODO: Update your variables here
		//----------------------------------------------------------------------------------

		// Draw
		//----------------------------------------------------------------------------------
		BeginTextureMode(render_texture);
		ClearBackground(BLANK);
		if(logoState.state != 4) {
			playLogoAnimation(&logoState);
		}
		else {
			document->ReloadStyleSheet();
			context->Update();

			render_interface.BeginFrame();
			context->Render();
			render_interface.EndFrame();
			rlLoadIdentity();

			//DrawText("Congrats! You created your first window!", 190, 200, 20, ORANGE);

			DrawFPS(10, 10);
		}
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
	func();
	return 0;
}
