all: jc

jc: hash.o tokenizer.o token.o jc.c 
	clang -g hash.o token.o tokenizer.o jc.c -o jc

tokenizer.o:
	clang -c tokenizer.c

token.o:
	clang -c token.c

hash.o:
	clang -c hash.c

clean:
	rm -rf *.o

clobber: clean
	rm -rf main
