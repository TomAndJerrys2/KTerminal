// KShell - a Unix shell for the KTerminal Project
//
// Kyle Brady 2025
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <dirent.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <ctype.h>

// Default macro sizes.
#define RL_BUFF_SIZE 1024
#define TK_BUFF_SIZE 64
#define TOK_DELIM " \t\r\n\a"

// Clear macro for gui.
char *clr[2] = {"clear", NULL};

//ANSI Color codes.
#define RED   		"\033[0;31m"
#define YELLOW 		"\033[0;33m"
#define CYAN 		"\033[0;36m"
#define GREEN 		"\033[0;32m"
#define BLUE 		"\033[0;34m"
#define INVERT		"\033[0;7m"
#define RESET  		"\e[0m" 
#define BOLD		"\e[1m"
#define ITALICS		"\e[3m"

// pipe history from command usage file.
void history_input(char **, char *);
void pipe_history_input(char *);

// print the inputted stream tokens.
void printtokens(char **);

// handling directory signals and passing the information to cd.
void get_dir(char *);
void signalHandler();

// Change directory as a builtin
int kshell_cd(char **);
 
// Default kshell exit function -> 0
int kshell_exit(char **);

// help command for learning builtins.
int kshell_help(char **);

// Unix Grep 
int kshell_grep(char **);

// launch the shell from the enviroment start point
int kshell_launch(char **);

// execute internal commands
int kshell_execute(char **);

// command usage history
int history_line_count();
int kshell_history();

// kshell pipe for terminal emulation on unix.
int kshell_pipe(char **);
int args_length(char **);

// split the line and read it.
char **split_line(char *);
char *read_line();

// trims whitespace around command string streams.
char *trimws(char *);	

// splitting pipes into tokens to be interpreted.
char **split_pipes(char *);

// getting the directory path of the command history file.
char *get_hist_file_path();

// Builtin function array
int (*builtin_funcs[])(char **) = {
		&kshell_cd, &kshell_help, &kshell_exit, &kshell_history, 
		&kshell_grep, &args_length 
};

// Builtin string equvilents for strcmp -> equivl
char *builtin_str[] = { 
	"cd",  "help", "exit" , "history", 
	"grep", "sizeof" 
};

// Library Functions by Kyle Brady
//
// As KShell and KTerminal function as my custom main drivers
// it's important to make somgething that's robust but also very
// customisable and lightweight. I've built a long of library tools
// that are included in the project alongside the terminal and shell
//
// vplayer - linux video player for all formats
// pview - photo viewer for linux for all formats
// docview - a document viewer for linux for all formats
// converter - a file type converter (txt, docx, pdf, png, jpeg, xlsx, etc...)
// server - a free multi-threaded web server built in c
// zipper - a linux tool used for zipping/unzipping archives
// keditor - a custom text editor for the kshell
// KSE - Kyle's Shell Extended
// KSE+ - Kyle's Shell Extended+
int (*library_funcs[])(char**) = {

	// &vplayer, &pview, &converter, &server, &docview
	// &zipper, &keditor, &kse, &kseplus
};

char *library_str[] = { 
	"vplayer",  "pview", "converter" , "server", 
	"docview", "zipper", "keditor"
};
