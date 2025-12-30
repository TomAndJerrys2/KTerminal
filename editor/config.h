#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <stdbool.h>
#include <ctype.h>
#include <errno.h>

#include "error.h"

struct termios editor;

// ##### Raw Input functions ##### //
inline void enable_raw_input(void);
inline void disable_raw_input(void);
void kill_editor(const char *);

// #### Editor Key Events #### //
char editor_read_key(void);
void editor_process_key(void);

/* ################################ */
//	      Editor Keys           //
/* ################################ */
#define CTRL_KEY(key) ((key) & 0x0f)

#endif
