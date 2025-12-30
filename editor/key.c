#include "config.h"

char editor_read_key() {
	i32 read_num;
	char input;

	while((read_num = read(STDIN_FILENO, & input, 1)) != 1)
		if(read_num == -1 && errno != EAGAIN)
			kill_editor("read");

	return input;
}

void editor_process_key() {
	char input = editor_read_key();

	switch(input) {
		case CTRL_KEY('q'):
			exit(0);
			break;
	}
}
