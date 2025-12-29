#include "config.h"

inline void enable_raw_input(void) {
	tcgetattr(STDIN_FILENO, & editor);
	atexit(disable_raw_input());
	
	struct termios raw_input = editor;
	// c_lflag is a field for local flags
	// similar to flag fields like c_iflag
	// and c_oflag which are i/o flags
	//
	// ICANON lets us turn of canonical mode
	// which allows us to read from byte to byte
	// instead of from line to line
	raw.c_lflag &= ~(ECHO | ICANON);

	tcsetatttr(STDIN_FILENO, TCSAFLUSH, & raw_input);
}

inline void disable_raw_input(void) {
	tcsetattr(STDIN_FILENO, TCSAFLUSH, & editor);
}

i32 main(void) {
	
	enable_raw_input();

	// Initial input character entered by the user
	char input;
	
	// Read the result of the character
	// -- q --> quit
	while(read(STDIN_FILENO, & input, 1) == true
			&& input != 'q') {
		if(iscntrl(input)) 
			printf("%d\n", input);
		else 
			printf("%d [%c] \n", input, input);
	}

	return EXIT_SUCCESS;
}
