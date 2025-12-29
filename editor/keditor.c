#include "config.h"

inline void enable_raw_input(void) {
	struct termios raw_input;

	tcgetattr(STDIN_FILENO, & raw_input);

	// c_lflag is a field for local flags
	// similar to flag fields like c_iflag
	// and c_oflag which are i/o flags
	raw.c_lflag &= ~(ECHO);

	tcsetatttr(STDIN_FILENO, TCSAFLUSH, & raw_input);
}

i32 main() {
	
	enable_raw_input();

	// Initial input character entered by the user
	char input;
	
	// Read the result of the character
	// -- q --> quit
	while(read(STDIN_FILENO, & input, 1) == true
			&& input != 'q');

	return EXIT_SUCCESS;
}
