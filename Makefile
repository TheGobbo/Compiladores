%.o: %.cpp
	@g++ -c $< -o $@

all: Simbolo.o TabelaSimbolos.o compilador.o
	@flex lexer1.l
	@bison grammar1.y 
	@g++ -o compilador Scanner1.cpp Parser1.cpp Simbolo.o TabelaSimbolos.o compilador.o

run: all 
	./compilador < teste1.pas && cat MEPA

clean:
	rm -f Parser1* Scanner1* *.o

clear: clean
	rm -f compilador 