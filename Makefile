

all: list shell

list: ls.c
	cc -o list.out ls.c

shell: shell.c
	cc -o shell.out shell.c

clean:
	rm *.out
