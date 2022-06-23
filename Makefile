build:
	g++ game.cpp RmlUi_Platform_Raylib.cpp RmlUi_Renderer_Raylib.cpp -lRmlCore -lfreetype -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
