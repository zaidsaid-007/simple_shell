#include "shell.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/types.h>

#define PROMPT "$"
#define MAX_LINE 256

void handle_command(char *command);
void handle_end_of_file(void);
void handle_error(char *error_message);
/**
 * main - function name
 *
 * Return: 0 on success
 */
int main(void)
{
	char line[MAX_LINE];
	char *prompt = PROMPT;

	while (1)
	{
		printf("%s", prompt);
		fgets(line, MAX_LINE, stdin);
		if (feof(stdin))
		{
			handle_end_of_file();
			break;
		}
		line[strlen(line) - 1] = '\0';
		handle_command(line);
	}
	return (0);
}
/**
 * handle_env - handles environment
 */
void handle_env(void)
{
	char **env = environ;

	while (*env != NULL)
	{
		printf("%s\n", *env);
		env++;
	}
}
/**
 * handle_command - function that handles the commands
 * @command: input command entered
 */
void handle_command(char *command)
{
	char *args[MAX_LINE];
	char *token;
	int args_count = 0, i;
	char *delim = " ";
	pid_t pid = fork();

	token = strtok(command, delim);
	while (token != NULL)
	{
		args[args_count++] = token;
		token = strtok(NULL, delim);
	}
	args[args_count] = NULL;

	printf("Command and arguments:\n");
	for (i = 0; i < args_count; i++)
	{
		printf("args[%d]: %s\n", i, args[i]);
	}
	if (strcmp(args[0], "exit") == 0)
	{
		exit(0);
	}
	else if (strcmp(args[0], "env") == 0)
	{
		handle_env();
	}
	if (pid == 0)
	{
		execlp(command, command, NULL);
		handle_error("Command not found");
		exit(1);
	}
	else if (pid > 0)
	{
		wait(NULL);
	}
	else
	{
		handle_error("Fork Failed");
	}
}
/**
 * handle_end_of_file - handles end of file
 */
void handle_end_of_file(void)
{
	printf("\n");
}
/**
 * handle_error - handles errors
 * @error_message: parameter
 */
void handle_error(char *error_message)
{
	printf("\nEror: %s\n", error_message);
}
/**
 * handle_exit - handles exit with specified status
 * @args: arguments to exit
 */
void handle_exit(char *args)
{
	int status = EXIT_SUCCESS;

	if (args != NULL)
	{
		status = atoi(args);
	}
	exit(status);
}
