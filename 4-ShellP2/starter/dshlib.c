#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "dshlib.h"
#define MAX_CMD_LENGTH 1024

/*
 * Implement your exec_local_cmd_loop function by building a loop that prompts the 
 * user for input.  Use the SH_PROMPT constant from dshlib.h and then
 * use fgets to accept user input.
 * 
 *      while(1){
 *        printf("%s", SH_PROMPT);
 *        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
 *           printf("\n");
 *           break;
 *        }
 *        //remove the trailing \n from cmd_buff
 *        cmd_buff[strcspn(cmd_buff,"\n")] = '\0';
 * 
 *        //IMPLEMENT THE REST OF THE REQUIREMENTS
 *      }
 * 
 *   Also, use the constants in the dshlib.h in this code.  
 *      SH_CMD_MAX              maximum buffer size for user input
 *      EXIT_CMD                constant that terminates the dsh program
 *      SH_PROMPT               the shell prompt
 *      OK                      the command was parsed properly
 *      WARN_NO_CMDS            the user command was empty
 *      ERR_TOO_MANY_COMMANDS   too many pipes used
 *      ERR_MEMORY              dynamic memory management failure
 * 
 *   errors returned
 *      OK                     No error
 *      ERR_MEMORY             Dynamic memory management failure
 *      WARN_NO_CMDS           No commands parsed
 *      ERR_TOO_MANY_COMMANDS  too many pipes used
 *   
 *   console messages
 *      CMD_WARN_NO_CMD        print on WARN_NO_CMDS
 *      CMD_ERR_PIPE_LIMIT     print on ERR_TOO_MANY_COMMANDS
 *      CMD_ERR_EXECUTE        print on execution failure of external command
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 1+)
 *      malloc(), free(), strlen(), fgets(), strcspn(), printf()
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 2+)
 *      fork(), execvp(), exit(), chdir()
 */
//helper function to handle quoted space correctly because 
//this is the only way without the original function breaking
//and I couldn't figure out build_cmd_buff so I did the same as my part3
void parse_command_with_quotes(char *cmd_buff, cmd_buff_t *cmd) {
    int i = 0;
    char *token;
    bool in_quotes = false;
    char temp_token[MAX_CMD_LENGTH];
    int token_len = 0;

    //tokenize the input here to handle the quotes
    for (int j = 0; cmd_buff[j] != '\0'; j++) {
        char c = cmd_buff[j];
        //toggle quotes
        if (c == '"') {
            in_quotes = !in_quotes;
        } else if (isspace(c) && !in_quotes) {
            if (token_len > 0) {
                temp_token[token_len] = '\0';
                cmd->argv[i++] = strdup(temp_token);
                token_len = 0;
            }
        } else {
            //add character to the current token
            temp_token[token_len++] = c;
        }
    }
    //add the last token if there's any
    if (token_len > 0) {
        temp_token[token_len] = '\0';
        cmd->argv[i++] = strdup(temp_token);
    }
    cmd->argc = i;
    cmd->argv[i] = NULL;
}

int exec_local_cmd_loop() {
    //have to allocte memory for `cmd_buff`
    char *cmd_buff = malloc(SH_CMD_MAX);
    if (cmd_buff == NULL) {
        perror("malloc");
        return ERR_MEMORY;
    }
    cmd_buff_t cmd;
    int rc = 0;
    //example given by professor
    while (1) {
        printf("%s", SH_PROMPT);
        if (fgets(cmd_buff, SH_CMD_MAX, stdin) == NULL) {
            printf("\n");
            break;
        }
        //removes the trailing newline character
        cmd_buff[strcspn(cmd_buff, "\n")] = '\0';

        cmd.argc = 0;
        cmd._cmd_buffer = cmd_buff;
        //parses the command input with quoted spaces
        //should be build_cmb_buff but I kept getting errors whenever I would `make`
        parse_command_with_quotes(cmd_buff, &cmd);
        //checks cmds
        if (cmd.argc > 0) {
            if (strcmp(cmd.argv[0], EXIT_CMD) == 0) {
                free(cmd_buff);
                return OK_EXIT;
            } else if (strcmp(cmd.argv[0], "cd") == 0) {
                if (cmd.argc == 1) {
                    printf("cd: missing argument\n");
                } else {
                    if (chdir(cmd.argv[1]) != 0) {
                        //invalid directory
                        perror("cd");
                    }
                }
                continue;
                //extra credit
            } else if (strcmp(cmd.argv[0], "dragon") == 0) {
                print_dragon();
                continue;
            }
        }
        //execute external command
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            free(cmd_buff);
            return ERR_MEMORY;
        } else if (pid == 0) {
            //child process
            if (execvp(cmd.argv[0], cmd.argv) == -1) {
                perror("execvp");
                free(cmd_buff);
                exit(ERR_EXEC_CMD);
            }
        } else {
            //parent process
            int status;
            waitpid(pid, &status, 0);
            if (WIFEXITED(status)) {
                rc = WEXITSTATUS(status);
            } else {
                rc = ERR_EXEC_CMD;
            }
        }
    }
    free(cmd_buff);
    //decided to handle the errors here rather then inside the loop like last assignment
    switch (rc) {
        case OK:
            //continues execution
            break;
        case ERR_MEMORY:
            printf("Memory allocation failed\n");
            break;
        case ERR_EXEC_CMD:
            printf("Command execution failed\n");
            break;
        case ERR_TOO_MANY_COMMANDS:
            printf("Too many commands\n");
            break;
        case ERR_CMD_OR_ARGS_TOO_BIG:
            printf("Command or arguments too large\n");
            break;
        case ERR_CMD_ARGS_BAD:
            printf("Bad command arguments\n");
            break;
        case OK_EXIT:
            //exits
            break;
        default:
            printf("Unknown error code: %d\n", rc);
            break;
    }
    return rc;
}
