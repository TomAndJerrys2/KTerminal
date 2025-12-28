kshell : kshell.c
	$(CC) kshell.c -o build/kshell -Wall -Wextra -03 -std=c99

coffee-editor : keditor.c
	$(CC) keditor.c -o build/keditor -Wall -Wextra -pedantic -03 -std=c99
