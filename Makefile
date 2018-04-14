CC=g++
CCFLAGS=-Wall -ansi -O2 -g -std=c++11
SOURCES=$(wildcard src/*.cpp src/**/*.cpp src/**/**/*.cpp)
OBJECTS=$(addprefix obj/, $(notdir $(SOURCES:.cpp=.o)))
TARGET=main

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) -o $@ $^

obj/%.o: **/%.cpp **/%.h
	$(CC) $(CCFLAGS) -o $@ -c $<

obj/%.o: **/**/%.cpp **/**/%.h
	$(CC) $(CCFLAGS) -o $@ -c $<

obj/%.o: **/**/**/%.cpp  **/**/**/%.h
	$(CC) $(CCFLAGS) -o $@ -c $<

obj/%.o: **/%.cpp
	$(CC) $(CCFLAGS) -o $@ -c $<

clean:
	rm -f obj/*.o $(TARGET)
