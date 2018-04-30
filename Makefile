
all: basic-stack-corruption-x86

clean:
	rm -f basic-stack-corruption-x86 

basic-stack-corruption-x86: basic-stack-corruption-x86.c
	gcc -g -O0 basic-stack-corruption-x86.c -o basic-stack-corruption-x86

