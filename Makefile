build:
	g++ game.cpp RmlUi_Platform_Raylib.cpp RmlUi_Renderer_Raylib.cpp -lRmlCore -lfreetype -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

raylib-desktop:
	mkdir -p vendor/tux/lib
	cd raylib/src && make clean && make PLATFORM=PLATFORM_DESKTOP
	cp raylib/src/libraylib.a vendor/tux/lib/

raylib-web:
	mkdir -p vendor/web/lib
	cd raylib/src && make clean && make PLATFORM=PLATFORM_WEB
	cp raylib/src/libraylib.a vendor/tux/lib/

rmlui-desktop:
	mkdir -p vendor/tux/lib
	cd vendor/tux/lib && cmake -DENABLE_PRECOMPILED_HEADERS=ON -DRmlUi_DIR=./ ../../../RmlUi && make

rmlui-web:
	mkdir -p vendor/web/lib
	cd vendor/web/lib && emcmake cmake -DRmlUi_DIR=./ -DBUILD_SHARED_LIBS=OFF ../../../RmlUi && make
