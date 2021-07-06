CC = clang++
SKULLS_SOURCE = ValleyOfBones.cpp
SKULLS_OUTPUT = ValleyOfBones.exe
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -O3 -std=c++17 -lstdc++fs
INCLUDES = -I/usr/include/SDL2

all: clean skulls

skulls:
	$(CC) $(SKULLS_SOURCE) $(LINKER_FLAGS) $(INCLUDES) -o $(SKULLS_OUTPUT)

clean:
	rm -f *.exe *.o
