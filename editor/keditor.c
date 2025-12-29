#include "config.h"
#include <unistd.h>
#include <stdbool.h>

i32 main() {
	
	// Initial input character entered by the user
	unsigned char input;
	
	// Read the result of the character
	// -- q --> quit
	while(read(STDIN_FILENO, & input, 1) == true
			&& input != 'q');

	return EXIT_SUCCESS;
}
