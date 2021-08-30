CC = clang++
LEGENDARY_KINGDOMS = LegendaryKingdoms.cpp
LEGENDARY_KINGDOMS_OUTPUT = LegendaryKingdoms.exe
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -O3 -std=c++17 -lstdc++fs
INCLUDES = -I/usr/include/SDL2

all: clean legendary-kingdoms

legendary-kingdoms:
	$(CC) -Wall $(LEGENDARY_KINGDOMS) $(LINKER_FLAGS) $(INCLUDES) -o $(LEGENDARY_KINGDOMS_OUTPUT)

debug:
	$(CC) -Wall $(LEGENDARY_KINGDOMS) $(LINKER_FLAGS) $(INCLUDES) -o $(LEGENDARY_KINGDOMS_OUTPUT) -DDEBUG

clean:
	rm -f *.exe *.o
