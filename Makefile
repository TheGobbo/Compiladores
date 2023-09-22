# VPATH := include/code_generation

# Source files (wildcard pattern)
# SRCS := src/compilador.cpp $(wildcard src/code_generation/*.cpp)

# Object files (wildcard pattern)
# OBJS := $(SRCS:.cpp=.o)

SRC_DIR = src


SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(SRC_DIR)/%.o,$(SRCS))

# Compiler
CXX := g++

# Compiler flags
CXXFLAGS := -std=c++11 -Iinclude

%.o: %.cpp
	$(CXX) -c $< -o $@

# Build the executable
all: $(OBJS)
	@flex $(SRC_DIR)/lexer.l
	@bison $(SRC_DIR)/grammar.y
	$(CXX) -o compilador Scanner.cpp Parser.cpp $(OBJS) $(CXXFLAGS)

# Run the compiled program
run: all
	./compilador < tests/input/aula10_escopo.pas

# Run tests
# test: all
# 	./tests/run_tests.sh (TODO)

# Clean object files and generated files
clean:
	rm -f Parser* Scanner* *.o compilador

# Clean all generated files, including the compiled program
clear: clean

