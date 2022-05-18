BUILD_DIR=build
LIB=lib
INCLUDE=include
SRC=src

game: $(BUILD_DIR) $(LIB) $(INCLUDE) $(SRC)
	g++ $(SRC)/tmp.cpp -o game -I $(INCLUDE) -L $(LIB) -l SDL2-2.0.0