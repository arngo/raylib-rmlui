
COMPILING_CORES ?= 2

TUX_DIR := vendor/tux
WEB_DIR := vendor/web
SRC_DIR := src

RMLUI_LIB := lib/RmlUi/libRmlCore.a
RAYLIB_LIB := lib/libraylib.a:

RMLUI_TUX_LIB := $(addprefix $(TUX_DIR), /lib/RmlUi/libRmlCore.a)
RAYLIB_TUX_LIB := $(addprefix $(TUX_DIR), /lib/libraylib.a)
RMLUI_WEB_LIB := $(addprefix $(WEB_DIR), /lib/RmlUi/libRmlCore.a)
RAYLIB_WEB_LIB := $(addprefix $(WEB_DIR), /lib/libraylib.a)


build/tux/game: $(SRC_DIR)/main.cpp $(RAYLIB_TUX_LIB) $(RMLUI_TUX_LIB)
	mkdir -p build/tux
	g++ -o build/tux/game $(SRC_DIR)/main.cpp rmlui_raylib_backend/RmlUi_Platform_Raylib.cpp rmlui_raylib_backend/RmlUi_Renderer_Raylib.cpp -Irmlui_raylib_backend/ -Iraylib/src/ -IRmlUi/Include/ $(RAYLIB_TUX_LIB) $(RMLUI_TUX_LIB) -lfreetype -lGL -lm -lpthread -ldl -lrt -lX11 -DPLATFORM_DESKTOP

build/web/index.html: $(SRC_DIR)/main.cpp $(RAYLIB_WEB_LIB) $(RMLUI_WEB_LIB)
	mkdir -p build/web
	em++ -o build/web/index.html $(SRC_DIR)/main.cpp rmlui_raylib_backend/RmlUi_Platform_Raylib.cpp rmlui_raylib_backend/RmlUi_Renderer_Raylib.cpp -Os -Wall -I. -Irmlui_raylib_backend/ -Iraylib/src/ -IRmlUi/Include/ -L. $(RAYLIB_WEB_LIB) $(RMLUI_WEB_LIB) -s USE_GLFW=3 -s ASYNCIFY --shell-file vendor/shell.html --preload-file assets -s TOTAL_MEMORY=67108864 -DPLATFORM_WEB -s USE_FREETYPE=1

$(RAYLIB_TUX_LIB):
	mkdir -p vendor/tux/lib
	#rm -f vendor/tux/lib/libraylib.a
	cd raylib/src && make clean && make PLATFORM=PLATFORM_DESKTOP
	cp raylib/src/libraylib.a $(RAYLIB_TUX_LIB)

$(RAYLIB_WEB_LIB):
	mkdir -p vendor/web/lib
	#rm -f vendor/web/lib/libraylib.a
	cd raylib/src && make clean && make PLATFORM=PLATFORM_WEB -e
	cp raylib/src/libraylib.a $(RAYLIB_WEB_LIB)

$(RMLUI_TUX_LIB):
	mkdir -p vendor/tux/lib/RmlUi/
	#rm -rf vendor/tux/lib/RmlUi/*
	cd vendor/tux/lib/RmlUi && cmake -DBUILD_SHARED_LIBS=OFF -DENABLE_PRECOMPILED_HEADERS=ON -DRmlUi_DIR=./ ../../../../RmlUi && make -j$(COMPILING_CORES)

$(RMLUI_WEB_LIB):
	mkdir -p vendor/web/lib/RmlUi/
	#rm -rf vendor/web/lib/RmlUi/*
	cd vendor/web/lib/RmlUi && emcmake cmake -DRmlUi_DIR=./ -DBUILD_SHARED_LIBS=OFF ../../../../RmlUi && make -j$(COMPILING_CORES)

.PHONY: run
run: build/tux/game
	build/tux/game

.PHONY: web
web: build/web/index.html

.PHONY: clean
clean:
	rm -r build

.PHONY: deep_clean
deep_clean: clean
	rm -r vendor/tux
	rm -r vendor/web
