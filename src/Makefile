CC = clang++
VALLEY_OF_BONES = ValleyOfBones.cpp
VALLEY_OF_BONES_OUTPUT = ValleyOfBones.exe
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -O3 -std=c++17 -lstdc++fs
INCLUDES = -I/usr/include/SDL2

all: clean valley-of-bones

valley-of-bones:
	$(CC) $(VALLEY_OF_BONES) $(LINKER_FLAGS) $(INCLUDES) -o $(VALLEY_OF_BONES_OUTPUT)

debug:
	$(CC) $(VALLEY_OF_BONES) $(LINKER_FLAGS) $(INCLUDES) -o $(VALLEY_OF_BONES_OUTPUT) -DDEBUG

clean:
	rm -f *.exe *.o