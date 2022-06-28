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

#include <raylib.h>
#include <RmlUi/Core.h>
//#include <RmlUi/Debugger.h>
#include <RmlUi_Renderer_Raylib.h>
#include <RmlUi_Platform_Raylib.h>
#include <rlgl.h>

struct UI {
	int speed = 0;
} ui;


bool SetupDataBinding(Rml::Context* context, Rml::DataModelHandle& my_model);

bool SetupDataBinding(Rml::Context* context, Rml::DataModelHandle& my_model)
{
	Rml::DataModelConstructor constructor = context->CreateDataModel("speedometer");
	if (!constructor)
		return false;

	constructor.Bind("speed", &ui.speed);

	my_model = constructor.GetModelHandle();

	return true;

}

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

	Rml::DataModelHandle model;

	SetTargetFPS(800);               // Set our game to run at 60 frames-per-second
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

	SetupDataBinding(context, model);

	//Rml::Debugger::Initialise(context);

	Rml::LoadFontFace("assets/fonts/exo.regular.otf");
	Rml::LoadFontFace("assets/fonts/patopian_1986.otf");
	//Rml::LoadFontFace("assets/fonts/Blobmoji.ttf");

	Rml::ElementDocument* document = context->LoadDocument("assets/rml/demo.rml");
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

		document->ReloadStyleSheet();
		ui.speed = GetFPS();
		model.DirtyVariable("speed");
		context->Update();
		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();

		ClearBackground(BLUE);

		render_interface.BeginFrame();

		context->Render();

		DrawText("Congrats! You created your first window!", 190, 200, 20, ORANGE);

		DrawFPS(10, 10);

		render_interface.EndFrame();
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

