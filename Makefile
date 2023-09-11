%.o: %.cpp
	@g++ -c $< -o $@

all: Simbolo.o TabelaSimbolos.o compilador.o MepaInterface.o
	@flex lexer.l
	@bison grammar.y 
	@g++ -o compilador Scanner.cpp Parser.cpp Simbolo.o TabelaSimbolos.o compilador.o MepaInterface.o

run: all 
	./compilador < aulas/aula10.pas
# && cat MEPA

clean:
	rm -f Parser* Scanner* *.o

clear: clean
	rm -f compilador 