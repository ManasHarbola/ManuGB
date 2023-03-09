CXX=g++
VERSION=-std=c++17
CXXFLAGS= $(VERSION) -Wall -O3
LDFLAGS=-lSDL2
SDL_INCLUDE=/opt/homebrew/include
LIB=/opt/homebrew/lib
SRC_INCLUDE=src
BUILD_DIR=build

MAKEFLAGS += -j4

INCLUDE=$(SDL_INCLUDE_DIR) $(MANUGB_INCLUDE_DIR)
SRC=src

OBJS = CPU.o MBC.o MBCFactory.o InstructionSet.o InterruptManager.o \
	   MMU.o PPU.o Timer.o GameBoy.o SDLDisplay.o Main.o

clean:
	rm $(BUILD_DIR)/*.o

debug: CXXFLAGS+=-g
debug-profile : CXXFLAGS+=-g -DPROFILE

debug: emulator
debug-profile: emulator

emulator: $(BUILD_DIR) $(LIB) $(SDL_INCLUDE) $(SRC_INCLUDE) $(SRC) $(OBJS)
	$(CXX) $(CXXFLAGS) -o ManuGB $(BUILD_DIR)/*.o -I$(SDL_INCLUDE) -I$(SRC_INCLUDE) -L$(LIB) $(LDFLAGS)

CPU.o : $(SRC)/cpu/CPU.cpp
	$(CXX) $(CXXFLAGS) -c $(SRC)/cpu/CPU.cpp -I $(SRC_INCLUDE) -o $(BUILD_DIR)/CPU.o

InstructionSet.o : $(SRC)/cpu/InstructionSet.cpp
	$(CXX) $(CXXFLAGS) -c $(SRC)/cpu/InstructionSet.cpp -I $(SRC_INCLUDE) -o $(BUILD_DIR)/InstructionSet.o

InterruptManager.o : $(SRC)/cpu/InterruptManager.cpp
	$(CXX) $(CXXFLAGS) -c $(SRC)/cpu/InterruptManager.cpp -I $(SRC_INCLUDE) -o $(BUILD_DIR)/InterruptManager.o

MMU.o : $(SRC)/mmu/MMU.cpp
	$(CXX) $(CXXFLAGS) -c $(SRC)/mmu/MMU.cpp -I $(SRC_INCLUDE) -o $(BUILD_DIR)/MMU.o

MBC.o : $(SRC)/mmu/mbc/MBC.cpp
	$(CXX) $(CXXFLAGS) -c $(SRC)/mmu/mbc/MBC.cpp -I $(SRC_INCLUDE) -o $(BUILD_DIR)/MBC.o

MBCFactory.o : $(SRC)/mmu/mbc/MBCFactory.cpp
	$(CXX) $(CXXFLAGS) -c $(SRC)/mmu/mbc/MBCFactory.cpp -I $(SRC_INCLUDE) -o $(BUILD_DIR)/MBCFactory.o

PPU.o : $(SRC)/ppu/PPU.cpp
	$(CXX) $(CXXFLAGS) -c $(SRC)/ppu/PPU.cpp -I $(SRC_INCLUDE) -o $(BUILD_DIR)/PPU.o

Timer.o : $(SRC)/timer/Timer.cpp
	$(CXX) $(CXXFLAGS) -c $(SRC)/timer/Timer.cpp -I $(SRC_INCLUDE) -o $(BUILD_DIR)/Timer.o

GameBoy.o : $(SRC)/GameBoy.cpp
	$(CXX) $(CXXFLAGS) -c $(SRC)/GameBoy.cpp -I $(SDL_INCLUDE) -I $(SRC_INCLUDE) -o $(BUILD_DIR)/GameBoy.o

Main.o : $(SRC)/Main.cpp
	$(CXX) $(CXXFLAGS) -c $(SRC)/Main.cpp -I $(SDL_INCLUDE) -I $(SRC_INCLUDE) -o $(BUILD_DIR)/Main.o

SDLDisplay.o : $(SRC)/graphics/SDLDisplay.cpp
	$(CXX) $(CXXFLAGS) -c $(SRC)/graphics/SDLDisplay.cpp -I $(SDL_INCLUDE) -I $(SRC_INCLUDE) -o $(BUILD_DIR)/SDLDisplay.o