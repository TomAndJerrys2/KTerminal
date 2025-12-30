#include "config.h"
#include "error.h"

void kill_editor(const char * msg) {
	perror(msg); exit(1);
}

inline void enable_raw_input(void) {
	if(tcgetattr(STDIN_FILENO, & editor) == -1)
		kill_editor("tcgetattr");

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

	if(tcsetatttr(STDIN_FILENO, TCSAFLUSH, & raw_input) == -1)
		kill_editor("tcsetattr");
}	

inline void disable_raw_input(void) {
	if(tcsetattr(STDIN_FILENO, TCSAFLUSH, & editor) == -1)
		kill_editor("tcsetattr");
} 

i32 main(void) {
	
	i32 is_running = 1;

	enable_raw_input();
	
	// Read the result of the character
	// -- q --> quit
	while(is_running) {

		if(read(STDIN_FILENO, & input, 1) == -1 
				&& errno != EAGAIN)
			kill_editor("read");

		// set as null terminator by default for memory purposes
		char input = '\0';
		if(iscntrl(input)) 
			printf("%d\r\n", input);
		else 
			printf("%d [%c] \r\n", input, input);

		if(input == 'q') break;
	}

	return EXIT_SUCCESS;
}
