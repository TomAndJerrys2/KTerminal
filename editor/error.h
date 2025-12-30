#ifndef ERROR_H
#define ERROR_H

// -- 16 KB --
#define BYTE_SIZE 16

typedef struct err_msg {
	// Error Message
	const char * message;
	// Count of Error Occured
	size_t       counter;
	// Where the Error(s) occured
	int 	     position[BYTE_SIZE];

} ErrorMessage;

ErrorMessage* create_message(const char*);

ErrorMessage error_message(const ErrorMessage);

const size_t return_err_count(const ErrorMessage, const int *);

#endif
