#include "kshell.h"

int builtin_funcs_count()
{
	return sizeof(builtin_str) / sizeof(char *);
}

void pipe_history_input(char *line)
{
	FILE *history_file = fopen(get_hist_file_path(), "a+");
	fprintf(history_file, "%d. %s\n", history_line_count(), line);

	fclose(history_file);
}

void history_input(char **args, char *d)
{	
	FILE *history_file = fopen(get_hist_file_path(), "a+");

	int j = 0;	

	fprintf(history_file, "%d. ", history_line_count());
	while(args[j] != NULL)
	{
		if(j > 0)
			fputs(d, history_file);

		fputs(args[j], history_file);
		j++;
	}
	fputs("\n", history_file);
	fclose(history_file);
}

char *trimws(char *str)
{
	char *end;

	while(isspace((unsigned char) *str)) str++;

	if(*str == 0)
		return str;

	end = str + strlen(str) - 1;

	while(end > str && isspace((unsigned char) *end)) end--;

	*(end+1) = 0;

	return str;
}

char **split_pipes(char *input)
{
	char *p = strtok(input, "|");
	char **s = malloc(1024*sizeof(char *));

	int i = 0;
	while(p != NULL)
	{
		
		s[i] = trimws(p);
		i++;
		p = strtok(NULL, "| ");
	}

	s[i] = NULL;
	i=0;

	while(s[i] != NULL)
	{
		printf("%s\n", s[i]);
		i++;
	}
	return s;
}

int args_length(char **args)
{
	int i = 0;

	while(args[i] != NULL)
	{
		i++;
	}
	return i;
}

// Main Shell pipe for emulation.
int kshell_pipe(char **args)
{
	// Saving current stdin and stdout for restoring
	int tempin=dup(0);			
	int tempout=dup(1);			
	int j=0, i=0, flag=0;
	int fdin = 0, fdout;

	for(j =0; j<args_length(args); j++)
	{
		
		if(strcmp(args[j], "<") == 0)
		{
			fdin=open(args[j+1], O_RDONLY);
			flag += 2;
		}
	}

	if(!fdin)
		fdin=dup(tempin);

	int pid;
	for(i=0; i<args_length(args)-flag; i++)
	{
		char **rargs = split_line(args[i]);

		dup2(fdin, 0);
		close(fdin);

		if(i == args_length(args)-3 && strcmp(args[i+1], ">") == 0)
		{	
			if((fdout = open(args[i+1], O_WRONLY)))
				i++;
		}
		else if(i == args_length(args)-flag-1)
			fdout = dup(tempout);
		else
		{
			int fd[2];
			pipe(fd);
			fdout = fd[1];
			fdin = fd[0];
		}	

		dup2(fdout, 1);
		close(fdout);
		
		
		pid = fork();
		if(pid == 0)
		{
			execvp(rargs[0], rargs);
			perror("kshell: error forking\n");
			exit(EXIT_FAILURE);
		}

		wait(NULL);
	}

	dup2(tempin, 0);
	dup2(tempout, 1);
	close(tempin);
	close(tempout);

	return 1;
}

char *get_hist_file_path()
{
	static char file_path[128];
	strcat(strncpy(file_path, getenv("HOME"), 113), "/.kshell_history");

	return file_path;
}



int kshell_history()
{	
	FILE *fp = fopen(get_hist_file_path(), "r");

	int ch, c, line_num = 1;
	char line[128];
	char prev_comm[128];

	char **args=NULL;

	if(!fp)
		fprintf(stderr, RED "kshell: file not found" RESET "\n");
	else
	{
		putchar('\n');
		while((c = getc(fp)) != EOF)
		{
			putchar(c);
		}
	}
	printf( "\n" INVERT " <0>: Quit    <#line>: Execute command    <-1>: clear history" RESET "\n\n: ");
	scanf("%d", &ch);

	getchar();
	fseek(fp, 0, SEEK_SET);

	if(isdigit(ch) != 0)
	{
		printf("please enter a numerical choice\n");	
	}
	else if (ch == 0)
	{	
		fclose(fp);

		return 1;
	}
	else if(ch == -1)
	{
		fclose(fp);
		fp = fopen(get_hist_file_path(), "w");
		fclose(fp);

		return kshell_execute(clr);
	}

	else
	{
		
	   	while((fgets(line, 128, fp)) != NULL)
	   	{
			if(line_num == ch)
			{

				
				strcpy(prev_comm, &line[3]);

				int p = 0, flag = 0;
				fclose(fp);

				while(prev_comm[p] != '\0')
				{
					if(prev_comm[p] == '|')
					{
						flag = 1;
						break;
					}
					p++;
				}
				if(!flag)
				{
					args = split_line(prev_comm);

					return kshell_launch(args);
				}
				else
				{
					args = split_pipes(prev_comm);

					return kshell_pipe(args);
				}
	
			}
			else
				line_num++;
				
	   	}
	}	
	return 1;
}
			
int history_line_count()
{
	FILE *fp = fopen(get_hist_file_path(), "r");

	int c;
	int numOfLines = 1;
	do
	{	
		c = getc(fp);
		if(c == '\n')
		{
			numOfLines++;	
		}

	} while(c != EOF);

	return numOfLines;
}

void signalHandler()
{
	signal(SIGINT, signalHandler);
	getchar();
}

int kshell_execute(char **args)
{
	pid_t cpid;
	int status;
	cpid = fork();

	if(cpid == 0)
	{	
		if(execvp(args[0], args) < 0)
			printf("kshell: command not found: %s\n", args[0]); 

		exit(EXIT_FAILURE);
		
	}
	else if(cpid < 0)
		printf(RED "kshell: error forking" RESET "\n");
	else
	{    
		waitpid(cpid, &status, WUNTRACED);
	}

	return 1;

}

int kshell_launch(char **args)
{
	int i = 0;
	if(args[0] == NULL)
	{
		return 1;
	}
	else if(strcmp(args[0], "history") 
			!= 0 && strcmp(args[0], "exit") 
				!= 0 && strcmp(args[0], "clear") != 0)		
	{
		history_input(args, " ");				
	}

	for(i = 0; i < builtin_funcs_count(); i++)
	{
		if(strcmp(args[0], builtin_str[i]) == 0)
		{	
			return (*builtin_funcs[i])(args);	
		}
	}

	return kshell_execute(args);

}


int kshell_grep(char **args)
{
	FILE *fp = NULL;
	int flag = 0;
	char temp[512];
	int line_num = 1;

	if(args[0] != NULL && strcmp(args[0], "grep") == 0)
	{
		if(args[1] != NULL && args[2] != NULL)
		{
			fp = fopen(args[2], "r");
			while((fgets(temp, 512, fp)) != NULL)
			{
				if(strstr(temp, args[1]))
				{
					printf("%d. %s", line_num, temp);
					flag = 1;
				}
				line_num++;
			}
			fclose(fp);
		}
		else
		{
			fprintf(stderr, RED "kshell: grep requires two params, " ITALICS "PATTERN" RESET RED " and " RED ITALICS "FILE" RESET "\n");
		}
	}
	if(flag == 0)
		printf("No matches were found \n");

	return 1;
}

int kshell_help(char **args)
{
	if(args[0] != NULL && strcmp(args[0], "help") == 0)
	{
		fprintf(stderr,"\n------\n" 
				BOLD "\nkshell >" RESET "Kyle's Linux shell and toolset\n"
				"\nSupported Command=:\n1. cd\n2. exit\n3. help\n4. touch\n5. cat\n"
				"6. rm\n7. dir\n8. rmdir\n9. fdisk\n10. ping\n11. pview\n12. vplayer\n"
				"13. converter\n14. server\n15. docview\n16. zipper\n17. keditor\n"
				"18. grep\n19. find\n19. ip\n20. macaddr\n21. route\n22. table\n 23. dd\n"
				"24. user\n25 "
				"\n\n------\n\n");
	}
	return 1;
}


int kshell_exit(char **args)
{
	return 0;
}

void get_dir(char *state)
{
	char cwd[1024];
	if(getcwd(cwd, sizeof(cwd)) != NULL)
	{
		if(strcmp(state, "loop") == 0)
			printf(RED "[ " RESET CYAN "%s" RESET RED " ] " RESET, cwd);

		else if(strcmp(state, "pwd") == 0)
			printf("%s\n", cwd);
	}
	else
	{
		printf("%sgetcwd() error%s", RED, RESET);
	}
}


int kshell_cd(char **args)
{
	if(args[1] == NULL)
	{
		fprintf(stderr, "%skshell: Please enter a path to cd%s\n", YELLOW, RESET);
	}
	else
	{
		if(chdir(args[1]) > 0)
		{
			perror("kshell");
		}
	}
	return 1;
}

char **split_line(char *line)
{
	int buffsize = TK_BUFF_SIZE, position = 0;

	char **tokens = malloc(buffsize*sizeof(char*));
	char *token;

	if(!tokens)
	{
		fprintf(stderr, "%skshell: Allocation error%s\n", RED, RESET);	
		exit(EXIT_FAILURE);
	}

	token = strtok(line, TOK_DELIM);
	while(token != NULL)
	{
		tokens[position] = token;
		position++;

		if(position>=buffsize)
		{
			buffsize += TK_BUFF_SIZE;
			tokens = realloc(tokens, buffsize*sizeof(char*));

			if(!tokens)
			{
				fprintf(stderr, "%skshell: Allocation error%s\n", RED, RESET);
				exit(EXIT_FAILURE);
			}
		}

		token = strtok(NULL, TOK_DELIM);
	}

	tokens[position] = NULL;

	return tokens;
}

// Links to debugger
void printtokens(char **tokens)
{
	int i = 0;
	while(tokens[i] != NULL)
	{
		printf("%s\n", tokens[i]);
		i++;
	}
}

char *read_line()
{
	int buffsize = RL_BUFF_SIZE;
	int position = 0;

	char *buffer = malloc(sizeof(char) * buffsize);
	int c;

	if(!buffer)
	{
		fprintf(stderr, "%skshell: Allocation error%s\n", RED, RESET);
		exit(EXIT_FAILURE);
	}

	while(1)
	{
		c  = getchar();
		if (c == EOF || c == '\n')
		{
			buffer[position] = '\0';
			return buffer;
		}
		else
		{
			buffer[position] = c;
		}
		position++;

		if (position >= buffsize)
		{
			printf("kshell: Overflow buffer // Allocating Memory...\n"); //test
			buffsize += RL_BUFF_SIZE;

			buffer = realloc(buffer, buffsize);

			if(!buffer)
			{
				fprintf(stderr, "%skshell: Allocation error%s\n", RED, RESET);
				exit(EXIT_FAILURE);
			}
		}
	}
}

// Main kshell enviroment for running process'.
void enviroment()
{
	char *line;
	char **args;
	int status=1, i = 0, flag = 0;
	

	do{
		get_dir("enviroment");
		printf(CYAN "kshell > " RESET);
		line = read_line();	

		flag = 0;

		i = 0;
		while(line[i] != '\0')
		{
			if(line[i] == '|')
			{	
				flag = 1;
				break;
			}
			i++;
		}
		if(flag)
		{
				pipe_history_input(line);
				args = split_pipes(line);	
				status = kshell_pipe(args);
		}
		else
		{
			args = split_line(line);
			status = kshell_launch(args);
		}

		free(line);
		free(args);

	}while(status);
}

// KShell entry point.
int main(int argc, char **argv)
{

	enviroment();

	return EXIT_SUCCESS;
}
