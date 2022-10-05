CC = g++ -O3
CC_FLAGS = -Wall -g
LD_FLAGS = -lGL -lGLU -lglut -lglfw -lassimp

EXEC = draw
SOURCES = $(wildcard src/*.cpp)
OBJECTS = $(SOURCES:.cpp=.o)
EXT = external/glad.o
WIDTH = 1920
HEIGHT = 1080

$(EXEC): $(OBJECTS)
	$(CC) $(OBJECTS) $(EXT) main.cpp -o $(EXEC) $(LD_FLAGS)

%.o: %.cpp
	$(CC) $(CC_FLAGS) -c $< -o $@

run: $(OBJECTS)
	$(CC) $(OBJECTS) $(EXT) main.cpp -o $(EXEC) $(LD_FLAGS)
	./$(EXEC) $(WIDTH) $(HEIGHT)

show: $(EXEC)
	./$(EXEC) $(WIDTH) $(HEIGHT) 

clean: 
	rm -f $(EXEC) $(OBJECTS)