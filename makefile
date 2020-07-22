export CXX=g++
export CXXFLAGS=-std=c++17 -pedantic-errors -Wall -Wextra -Wsign-conversion -Werror
export LIBS=-lSDL2 -lGL -lm -ldl -lpthread
export DBG?=-g
export OPTIMIZATION?=-O3 -march=native -flto


DuEngineDIR=DuEngine
_INLCUDE_F=dependencies/include dependencies/imgui .
INCLUDE_F=$(patsubst %, -I$(DuEngineDIR)/%, $(_INLCUDE_F))

LIBS_OBJ=libDuEngine.so
vdbg.o: visual_debugger.cpp emulador.cpp $(LIBS_OBJ)
	$(CXX) -o $@ visual_debugger.cpp emulador.cpp $(CXXFLAGS) $(OPTIMIZATION) $(DBG) $(INCLUDE_F) -L$(DuEngineDIR) -lDuEngine $(LIBS) -Wl,-rpath=$(DuEngineDIR)

.PHONY: clear Prepare clearAll DuEngineBuild
clear:
	rm -f Tetris.o

clearAll: clear
	$(MAKE) -C DuEngine clear
clearLib:
	$(MAKE) -C DuEngine clearAll
Prepare:
	$(MAKE) -C DuEngine Prepare

$(LIBS_OBJ): DuEngineBuild
DuEngineBuild:
	$(MAKE) -C DuEngine build

IMGUIDIR=dependencies/imgui
_IMGUISRC=imgui.cpp imgui_demo.cpp imgui_draw.cpp imgui_impl_opengl3.cpp imgui_impl_sdl.cpp imgui_widgets.cpp
IMGUISRC=$(patsubst %,$(IMGUIDIR)/%,$(_IMGUISRC))
