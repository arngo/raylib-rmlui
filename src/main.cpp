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

#include <iostream>
#include "raylib.h"
#include "RmlUi/Core.h"
//#include <RmlUi/Debugger.h>
#include "RmlUi_Renderer_Raylib.h"
#include "RmlUi_Platform_Raylib.h"
#include "rlgl.h"

#include "raylib_logo.hpp"
#include "input.hpp"
#include "event_listeners.hpp"
#include "globals.hpp"

RenderTexture2D render_texture;

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
	Rml::LoadFontFace("assets/fonts/DigitalDisco.ttf");

	Rml::ElementDocument* document = context->LoadDocument("assets/rml/demo.rml");
	if (document)
		document->Show();
	else
		return 0;

	auto resetListener = std::make_unique<ResetListener>();
	auto startListener = std::make_unique<StartListener>();
	auto menuAnimListener = std::make_unique<MenuAnimationListener>();
	Rml::Element *resetBtn = document->GetElementById("reset");
	Rml::Element *playBtn = document->GetElementById("play");
    Rml::Element *menu = document->GetElementById("menu");
	resetBtn->AddEventListener(Rml::EventId::Click, resetListener.get(), false);
	playBtn->AddEventListener(Rml::EventId::Click, startListener.get(), false);
    //menu->AddEventListener(Rml::EventId::Animationend, menuAnimListener.get(), false);


	SetTargetFPS(60);
	// Main game loop
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		// Update
		//----------------------------------------------------------------------------------
		// TODO: Update your variables here
		//----------------------------------------------------------------------------------
		ProcessMouseInput(context);


		// Draw
		//----------------------------------------------------------------------------------
		BeginTextureMode(render_texture);
		ClearBackground(BLANK);
		if(gameState == LOGO) {
			if(playLogoAnimation(&logoState)) {
				gameState = MENU;
				resetBtn->RemoveEventListener(Rml::EventId::Click, resetListener.get(), false);
				playBtn->RemoveEventListener(Rml::EventId::Click, startListener.get(), false);
				//menu->RemoveEventListener(Rml::EventId::Click, menuAnimListener.get(), false);
				document->Close();
				document = context->LoadDocument("assets/rml/demo.rml");
				document->Show();
				resetBtn = document->GetElementById("reset");
				playBtn = document->GetElementById("play");
				//menu = document->GetElementById("menu");
				resetBtn->AddEventListener(Rml::EventId::Click, resetListener.get(), false);
				playBtn->AddEventListener(Rml::EventId::Click, startListener.get(), false);
                //menu->AddEventListener(Rml::EventId::Animationend, menuAnimListener.get(), false);
			}
		}
		else if (gameState == MENU) {
			document->ReloadStyleSheet();
			context->Update();

			render_interface.BeginFrame();
			context->Render();
			render_interface.EndFrame();
			rlLoadIdentity();

            menu = document->GetElementById("menu");
            if (menuState == FADEOUT_START) {
                menuState = FADEOUT;
                menu->AddEventListener(Rml::EventId::Transitionend, menuAnimListener.get(), false);
                menu->SetProperty("opacity", "0");
                menu->SetProperty("animation", "1s cubic-out outro_main");
                resetBtn->RemoveEventListener(Rml::EventId::Click, resetListener.get(), false);
                playBtn->RemoveEventListener(Rml::EventId::Click, startListener.get(), false);
                context->ProcessMouseLeave();
                //menu->SetProperty("transition", "opacity 1s linear-out");
                //menu->SetProperty("background-color", "red");
                //menu->SetClass("fade", true);
                std::cout << "start fadeout" << std::endl;
            } else if (menuState == INVISIBLE) {
                menu->RemoveEventListener(Rml::EventId::Transitionend, menuAnimListener.get(), false);
				document->Close();
                gameState = GAME;
            }

			//DrawText("Congrats! You created your first window!", 190, 200, 20, ORANGE);

			//DrawFPS(10, 10);
        } else if (gameState == GAME) {

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

