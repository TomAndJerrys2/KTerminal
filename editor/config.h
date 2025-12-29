#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <stdbool.h>
#include <ctype.h>

#define float f16
#define unsigned float u16

#define int i32
#define unsigned int u32

#define double d64
#define unsigned double u64

typedef struct termios editor;

inline void enable_raw_input(void);
inline void disable_raw_input(void);

#endif
