BUILD_DIR=build
SDL_INCLUDE=include
SRC_INCLUDE=src
LIB=lib
INCLUDE=$(SDL_INCLUDE_DIR) $(MANUGB_INCLUDE_DIR)
SRC=src
VERSION=-std=c++17

OBJS = CPU.o InstructionSet.o InterruptManager.o \
	   MMU.o PPU.o Timer.o GameBoy.o SDLDisplay.o Main.o

game: $(BUILD_DIR) $(LIB) $(SDL_INCLUDE) $(SRC)/tmp.cpp
	g++ $(VERSION) -O3 -o game $(SRC)/tmp.cpp -I $(SDL_INCLUDE) -L $(LIB) -l SDL2-2.0.0

debug: $(BUILD_DIR) $(LIB) $(SDL_INCLUDE) $(SRC_INCLUDE) $(SRC) $(OBJS)
	g++ $(VERSION) -g -O3 -o ManuGB $(BUILD_DIR)/*.o -I $(SDL_INCLUDE) -I $(SRC_INCLUDE) -L $(LIB) -l SDL2-2.0.0

CPU.o : $(SRC)/cpu/CPU.cpp
	g++ $(VERSION) -Wall -O3 -c -g $(SRC)/cpu/CPU.cpp -I $(SRC_INCLUDE) -o $(BUILD_DIR)/CPU.o

InstructionSet.o : $(SRC)/cpu/InstructionSet.cpp
	g++ $(VERSION) -Wall -O3 -c -g $(SRC)/cpu/InstructionSet.cpp -I $(SRC_INCLUDE) -o $(BUILD_DIR)/InstructionSet.o

InterruptManager.o : $(SRC)/cpu/InterruptManager.cpp
	g++ $(VERSION) -Wall -O3 -c -g $(SRC)/cpu/InterruptManager.cpp -I $(SRC_INCLUDE) -o $(BUILD_DIR)/InterruptManager.o

MMU.o : $(SRC)/mmu/MMU.cpp
	g++ $(VERSION) -Wall -O3 -c -g $(SRC)/mmu/MMU.cpp -I $(SRC_INCLUDE) -o $(BUILD_DIR)/MMU.o

PPU.o : $(SRC)/ppu/PPU.cpp
	g++ $(VERSION) -Wall -O3 -c -g $(SRC)/ppu/PPU.cpp -I $(SRC_INCLUDE) -o $(BUILD_DIR)/PPU.o

Timer.o : $(SRC)/timer/Timer.cpp
	g++ $(VERSION) -Wall -O3 -c -g $(SRC)/timer/Timer.cpp -I $(SRC_INCLUDE) -o $(BUILD_DIR)/Timer.o

GameBoy.o : $(SRC)/GameBoy.cpp
	g++ $(VERSION) -Wall -O3 -c -g $(SRC)/GameBoy.cpp -I $(SDL_INCLUDE) -I $(SRC_INCLUDE) -o $(BUILD_DIR)/GameBoy.o

Main.o : $(SRC)/Main.cpp
	g++ $(VERSION) -Wall -O3 -c -g $(SRC)/Main.cpp -I $(SDL_INCLUDE) -I $(SRC_INCLUDE) -o $(BUILD_DIR)/Main.o

SDLDisplay.o : $(SRC)/graphics/SDLDisplay.cpp
	g++ $(VERSION) -Wall -O3 -c -g $(SRC)/graphics/SDLDisplay.cpp -I $(SDL_INCLUDE) -I $(SRC_INCLUDE) -o $(BUILD_DIR)/SDLDisplay.o