kshell : kshell.c
	$(CC) kshell.c -o build/kshell -Wall -Wextra -std=c99

coffee-editor : keditor.c
	$(CC) keditor.c -o build/keditor -Wall -Wextra -pedantic -std=c99
