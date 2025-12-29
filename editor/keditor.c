#include "config.h"
#include "error.h"

inline void enable_raw_input(void) {
	tcgetattr(STDIN_FILENO, & editor);
	atexit(disable_raw_input());
	
	struct termios raw_input = editor;

	// IXON is an old flag for transmisson operations
	// XOFF to pause and XON will resume the transmission
	raw_input.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
	
	// OPOST disables post-processing of output streams
	raw_input.c_oflag &= ~(OPOST);

	// CS8 defines a character size of 8bits per byte
	// this is default but its good to make sure on UNIX
	//
	// passed via a bitwise operation as CS8 is not a flag
	// but a bit mask
	raw_input.cflag |= (CS8);	

	// c_lflag is a field for local flags
	// similar to flag fields like c_iflag
	// and c_oflag which are i/o flags
	//
	// ICANON lets us turn of canonical mode
	// which allows us to read from byte to byte
	// instead of from line to line
	raw_input.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
	
	raw_input.c_cc[VMIN] = 0;
	raw_input.c_cc[VTIME] = 1;

	tcsetatttr(STDIN_FILENO, TCSAFLUSH, & raw_input);
}	

inline void disable_raw_input(void) {
	tcsetattr(STDIN_FILENO, TCSAFLUSH, & editor);
}

i32 main(void) {
	
	enable_raw_input();
	
	// Read the result of the character
	// -- q --> quit
	while(read(STDIN_FILENO, & input, 1) == true && input != 'q') {
		// set as null terminator by default for memory purposes
		char input = '\0';


		if(iscntrl(input)) 
			printf("%d\r\n", input);
		else 
			printf("%d [%c] \r\n", input, input);
	}

	return EXIT_SUCCESS;
}
