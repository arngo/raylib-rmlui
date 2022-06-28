build-desktop:
	mkdir -p build/tux
	g++ -o build/tux/game game.cpp rmlui_raylib_backend/RmlUi_Platform_Raylib.cpp rmlui_raylib_backend/RmlUi_Renderer_Raylib.cpp -Irmlui_raylib_backend/ -Iraylib/src/ -IRmlUi/Include/ vendor/tux/lib/libraylib.a vendor/tux/lib/RmlUi/libRmlCore.a -lfreetype -lGL -lm -lpthread -ldl -lrt -lX11 -DPLATFORM_DESKTOP

build-web:
	mkdir -p build/web
	em++ -o build/web/index.html game.cpp rmlui_raylib_backend/RmlUi_Platform_Raylib.cpp rmlui_raylib_backend/RmlUi_Renderer_Raylib.cpp -Os -Wall -I. -Irmlui_raylib_backend/ -Iraylib/src/ -IRmlUi/Include/ -L. vendor/web/lib/libraylib.a vendor/web/lib/RmlUi/libRmlCore.a -s USE_GLFW=3 -s ASYNCIFY --shell-file vendor/shell.html --preload-file assets -s TOTAL_MEMORY=67108864 -DPLATFORM_WEB -s USE_FREETYPE=1

raylib-desktop:
	mkdir -p vendor/tux/lib
	rm -f vendor/tux/lib/libraylib.a
	cd raylib/src && make clean && make PLATFORM=PLATFORM_DESKTOP
	cp raylib/src/libraylib.a vendor/tux/lib/

raylib-web:
	mkdir -p vendor/web/lib
	rm -f vendor/web/lib/libraylib.a
	cd raylib/src && make clean && make PLATFORM=PLATFORM_WEB -e
	cp raylib/src/libraylib.a vendor/web/lib/

rmlui-desktop:
	mkdir -p vendor/tux/lib/RmlUi/
	rm -rf vendor/tux/lib/RmlUi/*
	cd vendor/tux/lib/RmlUi && cmake -DBUILD_SHARED_LIBS=OFF -DENABLE_PRECOMPILED_HEADERS=ON -DRmlUi_DIR=./ ../../../../RmlUi && make -j2

rmlui-web:
	mkdir -p vendor/web/lib/RmlUi/
	rm -rf vendor/web/lib/RmlUi/*
	cd vendor/web/lib/RmlUi && emcmake cmake -DRmlUi_DIR=./ -DBUILD_SHARED_LIBS=OFF ../../../../RmlUi && make -j2
