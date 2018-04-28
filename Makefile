
all: basic-stack-corruption-defeats-stack-protector-x86-64 basic-stack-corruption-x86-64

clean:
	rm -f basic-stack-corruption-x86-64 basic-stack-corruption-defeats-stack-protector-x86-64

basic-stack-corruption-x86-64: basic-stack-corruption-x86-64.c
	gcc basic-stack-corruption-x86-64.c -o basic-stack-corruption-x86-64

basic-stack-corruption-defeats-stack-protector-x86-64: basic-stack-corruption-defeats-stack-protector-x86-64.c
	gcc -fstack-protector-strong basic-stack-corruption-defeats-stack-protector-x86-64.c -o basic-stack-corruption-defeats-stack-protector-x86-64
