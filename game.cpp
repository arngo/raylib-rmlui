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
#include <cmath>

struct UI {
	int speed = 0;
	bool tachbar[15];
} ui;


bool SetupDataBinding(Rml::Context* context, Rml::DataModelHandle& my_model);

bool SetupDataBinding(Rml::Context* context, Rml::DataModelHandle& my_model)
{
	Rml::DataModelConstructor constructor = context->CreateDataModel("speedometer");
	if (!constructor)
		return false;

	constructor.Bind("speed", &ui.speed);
	constructor.Bind("tachbar1", &ui.tachbar[0]);
	constructor.Bind("tachbar2", &ui.tachbar[1]);
	constructor.Bind("tachbar3", &ui.tachbar[2]);
	constructor.Bind("tachbar4", &ui.tachbar[3]);
	constructor.Bind("tachbar5", &ui.tachbar[4]);
	constructor.Bind("tachbar6", &ui.tachbar[5]);
	constructor.Bind("tachbar7", &ui.tachbar[6]);
	constructor.Bind("tachbar8", &ui.tachbar[7]);
	constructor.Bind("tachbar9", &ui.tachbar[8]);
	constructor.Bind("tachbar10", &ui.tachbar[9]);
	constructor.Bind("tachbar11", &ui.tachbar[10]);
	constructor.Bind("tachbar12", &ui.tachbar[11]);
	constructor.Bind("tachbar13", &ui.tachbar[12]);
	constructor.Bind("tachbar14", &ui.tachbar[13]);
	constructor.Bind("tachbar15", &ui.tachbar[14]);

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

	//SetTargetFPS(1200);               // Set our game to run at 60 frames-per-second
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
		double sin = ((std::sin(GetTime()*1.3) + 1) / 2) * 1500;
		ui.speed = sin;
		for (int i = 0; i < 15; i++) {
			if(sin > ((i * 100))) {
				ui.tachbar[i] = true;
			} else {
				ui.tachbar[i] = false;
			}
		}

		model.DirtyVariable("tachbar1");
		model.DirtyVariable("tachbar2");
		model.DirtyVariable("tachbar3");
		model.DirtyVariable("tachbar4");
		model.DirtyVariable("tachbar5");
		model.DirtyVariable("tachbar6");
		model.DirtyVariable("tachbar7");
		model.DirtyVariable("tachbar8");
		model.DirtyVariable("tachbar9");
		model.DirtyVariable("tachbar10");
		model.DirtyVariable("tachbar11");
		model.DirtyVariable("tachbar12");
		model.DirtyVariable("tachbar13");
		model.DirtyVariable("tachbar14");
		model.DirtyVariable("tachbar15");

		model.DirtyVariable("speed");
		context->Update();
		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();

		ClearBackground(DARKGRAY);

		render_interface.BeginFrame();
		context->Render();
		render_interface.EndFrame();

		DrawRectangle(5, 5, 100, 25, BLACK);
		DrawText("This text is drawn from Raylib", 150, 175, 20, ORANGE);
		DrawText("The FPS counter and the background is too", 150, 200, 20, ORANGE);
		DrawText("The UI however is drawn with RmlUI!", 150, 225, 20, ORANGE);

		DrawFPS(10, 10);

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

