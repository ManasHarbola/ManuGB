BUILD_DIR=build
LIB=lib
INCLUDE=include
SRC=src
VERSION=-std=c++14

OBJS = CPU.o InstructionHandler.o InterruptManager.o \
	   MMU.o PPU.o Timer.o GameBoy.o Main.o

game: $(BUILD_DIR) $(LIB) $(INCLUDE) $(SRC)
	g++ $(SRC)/tmp.cpp -o game -I $(INCLUDE) -L $(LIB) -l SDL2-2.0.0

debug: $(BUILD_DIR) $(LIB) $(INCLUDE) $(SRC) $(OBJS)
	g++ $(VERSION) -g -o ManuGB $(BUILD_DIR)/*.o -I $(INCLUDE) -L $(LIB) -l SDL2-2.0.0

CPU.o : $(SRC)/cpu/CPU.cpp
	g++ $(VERSION) -Wall -c -g $(SRC)/cpu/CPU.cpp -o $(BUILD_DIR)/CPU.o

InstructionHandler.o : $(SRC)/cpu/InstructionHandler.cpp
	g++ $(VERSION) -Wall -c -g $(SRC)/cpu/InstructionHandler.cpp -o $(BUILD_DIR)/InstructionHandler.o

InterruptManager.o : $(SRC)/cpu/InterruptManager.cpp
	g++ $(VERSION) -Wall -c -g $(SRC)/cpu/InterruptManager.cpp -o $(BUILD_DIR)/InterruptManager.o

MMU.o : $(SRC)/mmu/MMU.cpp
	g++ $(VERSION) -Wall -c -g $(SRC)/mmu/MMU.cpp -o $(BUILD_DIR)/MMU.o

PPU.o : $(SRC)/ppu/PPU.cpp
	g++ $(VERSION) -Wall -c -g $(SRC)/ppu/PPU.cpp -o $(BUILD_DIR)/PPU.o

Timer.o : $(SRC)/timer/Timer.cpp
	g++ $(VERSION) -Wall -c -g $(SRC)/timer/Timer.cpp -o $(BUILD_DIR)/Timer.o

GameBoy.o : $(SRC)/GameBoy.cpp
	g++ $(VERSION) -Wall -c -g $(SRC)/GameBoy.cpp -I $(INCLUDE) -o $(BUILD_DIR)/GameBoy.o

Main.o : $(SRC)/Main.cpp
	g++ $(VERSION) -Wall -c -g $(SRC)/Main.cpp -I $(INCLUDE) -o $(BUILD_DIR)/Main.o

