#ifndef ERROR_H
#define ERROR_H

typedef struct {
	
	const char * message;
	size_t       counter;

} ErrorMessage;

ErrorMessage * create_message(const char *);

ErrorMessage error_message(const ErrorMessage &);

#endif
