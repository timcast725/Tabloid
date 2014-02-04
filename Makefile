
# The compiler to use
CC = clang++
# All source files are located in src
SOURCES = $(wildcard src/*.cpp)
# All source objects and make file objects
OBJECTS = $(addprefix obj/, $(notdir $(SOURCES:.cpp=.o)))
# Flags to use when compiling
CFLAGS = -Wall -c -std=c++0x
# Flags to use when linking
LDFLAGS = -laubio -laubioext -W1,-rpath=/usr/local/lib
# Executable name
EXECUTABLE = Tab

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $^ -o $@

obj/%.o: src/%.cpp
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm obj/*.o obj/*.d $(EXECUTABLE)

# Recompile when a dependent header is updated.
CFLAGS += -MMD
-include $(OBJECTS:.o=.d)
