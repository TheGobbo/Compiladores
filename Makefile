# VPATH := include/code_generation

# Source files (wildcard pattern)
SRCS := $(wildcard src/code_generation/*.cpp)  src/compilador.cpp

# Object files (wildcard pattern)
OBJS := $(SRCS:.cpp=.o)

# Compiler
CXX := g++

# Compiler flags
CXXFLAGS :=  -I./include -I./src

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Build the executable
all: $(OBJS)
	@flex src/lexer.l
	@bison src/grammar.y
	@$(CXX) $(CXXFLAGS) -o compilador Scanner.cpp Parser.cpp $(OBJS) 

# Run the compiled program
run: all
	./compilador < tests/input/varsGlobais.pas

# Run tests
test: all
	./tests/run_tests.sh

# Clean object files and generated files
clean:
	rm -f *.o Parser* Scanner*
	find tests/input/ -type f ! -name "*.pas" -exec rm -f {} \;

# Clean all generated files, including the compiled program
clear: clean
	rm -f compilador
