%.o: %.cpp
	@g++ -c $< -o $@

all: Simbolo.o TabelaSimbolos.o compilador.o MepaInterface.o
	@flex lexer.l
	@bison grammar.y 
	@g++ -o compilador Scanner.cpp Parser.cpp Simbolo.o TabelaSimbolos.o compilador.o MepaInterface.o

run: all 
	./compilador < tests/input/aula10_escopo.pas
	
test: all
	./run_tests.sh
# && cat MEPA

clean:
	rm -f Parser* Scanner* *.o

clear: clean
	rm -f compilador 