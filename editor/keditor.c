// Keditor -- In working progress :))
#include <stdint.h> 
#include "config.h"

#define i8 int8_t

#define i16 int16_t
#define i32 int32_t

#define u16 uint16_t
#define u32 uint32_t

void kill_editor(const char * msg) {
	perror(msg); exit(1);
}

inline void disable_raw_input(void) {
	if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &editor) == -1)
		kill_editor("tcsetattr");
} 

inline void enable_raw_input(void) {
	if(tcgetattr(STDIN_FILENO, &editor) == -1)
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
	raw_input.c_cflag |= (CS8);	

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

	if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw_input) == -1)
		kill_editor("tcsetattr");
}	

i8 main() {
	
	i8 is_running = 1;

	enable_raw_input();
	
	// Read the result of the character
	// -- q --> quit
	while(is_running) {
		editor_process_key();
	}

	return EXIT_SUCCESS;
}
