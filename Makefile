CC=g++
CFLAGS=-lglfw -lGL -lGLU -lglut -lassimp
OBJECTS=glad/glad.o
WIDTH=1920
HEIGHT=1080

compile: main.cpp src/*.cpp
	$(CC) -o draw main.cpp src/*.cpp $(OBJECTS) $(CFLAGS) 

run: main.cpp src/*.cpp
	$(CC) -o draw main.cpp src/*.cpp $(OBJECTS) $(CFLAGS) 
	./draw $(WIDTH) $(HEIGHT)

draw: draw
	./draw $(WIDTH) $(HEIGHT)

clean:
	rm -f draw