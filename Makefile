# VPATH := include/code_generation

# Source files (wildcard pattern)
SRCS := $(wildcard src/code_generation/*.cpp)  src/compilador.cpp

# Object files (wildcard pattern)
OBJS := $(SRCS:.cpp=.o)

# Compiler (use c++14 or higher)
CXX := g++ --std=c++14

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
# ./compilador < tests/input/aula05_globais.pas
# ./compilador < tests/input/aula08_while.pas
# ./compilador < tests/input/aula09_if.pas
# ./compilador < tests/input/aula10_escopo.pas
# ./compilador < tests/input/aula10_proc.pas
# ./compilador < tests/input/aula11_pf.pas
	./compilador < tests/input/aula11_stress.pas
# ./compilador < tests/input/aula12_var.pas
# ./compilador < tests/input/aula13_func.pas
# ./compilador < tests/input/tests.pas

mepa: 
	./executaMepa -r -i output/output.mepa

# Run tests
test: all
	./tests/run_tests.sh

# Clean object files and generated files
clean:
# remove .o from src
	@find src/ -type f ! -regex ".*\.\(cpp\|y\|l\|hpp\)" -exec rm -f {} \; 
# remove compiled pascal from tests/
	@find tests/input/ -type f ! -name "*.pas" -exec rm -f {} \;
# remove lexer and bison result files
	@rm -f Parser* Scanner*

# Clean all generated files, including the compiled program
clear: clean
	rm -f compilador
