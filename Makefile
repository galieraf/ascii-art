# Used commands
DOXYGEN = doxygen
CXX = g++
LD = g++
CXXFLAGS = -g -std=c++17 -Wall -pedantic -I./src -I/usr/include
LDFLAGS += -lpng -ljpeg -lncurses

# Directories for source files and object files and doc
SRC_DIR = src
OBJ_DIR = obj
DOC_DIR = doc
# Source files
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
# Object files
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SOURCES))

# Include directories
INCLUDES = -I.

# Executable file name
EXECUTABLE = galieraf

# Generate dependencies
DEPFILES = $(OBJECTS:.o=.d)
-include $(DEPFILES)

# Rule for creating the executable
$(EXECUTABLE): $(OBJECTS)
	$(LD) $(OBJECTS) -o $@ $(LDFLAGS)

# Rule for creating object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(INCLUDES) -MMD -c $< -o $@ $(CXXFLAGS)

# Rule for creating the object directory
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Default target
.DEFAULT_GOAL := all
# "all" target
all: compile doc
# "compile" target
compile: $(EXECUTABLE)
# "doc" target
doc:
	$(DOXYGEN) ./Doxyfile

# "run" target
run:
	./$(EXECUTABLE) $(ARG)

# "clean" target
clean:
	rm -rf $(OBJ_DIR) $(EXECUTABLE) $(DOC_DIR)
