CC=clang++
LEGENDARY_KINGDOMS=LegendaryKingdoms.cpp
LEGENDARY_KINGDOMS_OUTPUT=LegendaryKingdoms.exe
LINKER_FLAGS=-O3 -std=c++17 -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
INCLUDES=-I/usr/include/SDL2

UNAME_S=$(shell uname -s)

ifeq ($(UNAME_S),Darwin)
        LINKER_FLAGS += -stdlib=libc++
        INCLUDES += -I/usr/local/include/SDL2
else
        LINKER_FLAGS += -lstdc++fs
endif

all: clean legendary-kingdoms

legendary-kingdoms:
	$(CC) -Wall $(LEGENDARY_KINGDOMS) $(LINKER_FLAGS) $(INCLUDES) -o $(LEGENDARY_KINGDOMS_OUTPUT)

debug:
	$(CC) -Wall $(LEGENDARY_KINGDOMS) $(LINKER_FLAGS) $(INCLUDES) -o $(LEGENDARY_KINGDOMS_OUTPUT) -DDEBUG

clean:
	rm -f *.exe *.o
