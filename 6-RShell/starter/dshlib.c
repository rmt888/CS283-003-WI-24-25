#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "dshlib.h"



#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "dshlib.h"

/**** 
 **** FOR REMOTE SHELL USE YOUR SOLUTION FROM SHELL PART 3 HERE
 **** THE MAIN FUNCTION CALLS THIS ONE AS ITS ENTRY POINT TO
 **** EXECUTE THE SHELL LOCALLY
 ****
 */

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
int build_cmd_list(char *cmd_buff, command_list_t *cmd_list) {
    int cmd_index = 0, arg_index = 0;
    bool in_quotes = false;
    char temp_token[CMD_MAX];
    int token_len = 0;

    for (int j = 0; cmd_buff[j] != '\0'; j++) {
        char c = cmd_buff[j];
        if (c == '"') {
            in_quotes = !in_quotes;
        } else if (c == '|' && !in_quotes) {
            if (token_len > 0) {
                temp_token[token_len] = '\0';
                cmd_list->commands[cmd_index].argv[arg_index++] = strdup(temp_token);
                token_len = 0;
            }
            cmd_list->commands[cmd_index].argc = arg_index;
            cmd_list->commands[cmd_index].argv[arg_index] = NULL;
            cmd_index++;
            arg_index = 0;
        } else if (isspace(c) && !in_quotes) {
            if (token_len > 0) {
                temp_token[token_len] = '\0';
                cmd_list->commands[cmd_index].argv[arg_index++] = strdup(temp_token);
                token_len = 0;
            }
        } else {
            temp_token[token_len++] = c;
        }
    }
    if (token_len > 0) {
        temp_token[token_len] = '\0';
        cmd_list->commands[cmd_index].argv[arg_index++] = strdup(temp_token);
    }
    cmd_list->commands[cmd_index].argc = arg_index;
    cmd_list->commands[cmd_index].argv[arg_index] = NULL;

    return OK;
}

Built_In_Cmds match_command(const char *input) {
    if (strcmp(input, "exit") == 0) {
        return BI_CMD_EXIT;
    } else if (strcmp(input, "cd") == 0) {
        return BI_CMD_CD;
    } else if (strcmp(input, "help") == 0) {
        return BI_CMD_CD;
    }
    return BI_NOT_BI;
}

int free_cmd_buff(cmd_buff_t *cmd_buff) {
    if (!cmd_buff) return ERR_MEMORY;
    free(cmd_buff->_cmd_buffer);
    cmd_buff->_cmd_buffer = NULL;
    return OK;
}

int free_cmd_list(command_list_t *cmd_list) {
    for (int i = 0; i < cmd_list->num; i++) {
        for (int j = 0; j < cmd_list->commands[i].argc; j++) {
            free(cmd_list->commands[i].argv[j]);
        }
    }
    return OK;
}

Built_In_Cmds exec_built_in_cmd(cmd_buff_t *cmd) {
    if (strcmp(cmd->argv[0], "cd") == 0) {
        if (cmd->argc < 2) {
            fprintf(stderr, "cd: missing argument\n");
            return BI_CMD_CD;
        }
        if (chdir(cmd->argv[1]) != 0) {
            perror("cd");
        }
        return BI_CMD_CD;
    } else if (strcmp(cmd->argv[0], "help") == 0) {
        printf("Supported commands: cd, exit, help\n");
        return BI_CMD_CD;
    }
    return BI_NOT_BI;
}

int exec_cmd(cmd_buff_t *cmd) {
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return ERR_MEMORY;
    } else if (pid == 0) {
        execvp(cmd->argv[0], cmd->argv);
        perror("execvp");
        exit(EXIT_FAILURE);
    }
    wait(NULL);
    return OK;
}

int exec_cmd_and_capture_output(cmd_buff_t *cmd, char **output) {
    int pipefd[2];
    pid_t pid;
    char buffer[CMD_MAX];
    int len;

    if (pipe(pipefd) == -1) {
        perror("pipe");
        return ERR_MEMORY;
    }

    pid = fork();
    if (pid < 0) {
        perror("fork");
        return ERR_MEMORY;
    } else if (pid == 0) {

        //child process
        //closes read end of pipe
        close(pipefd[0]);
        //redirects stdout to pipe
        dup2(pipefd[1], STDOUT_FILENO);
        execvp(cmd->argv[0], cmd->argv);
        perror("execvp");
        exit(EXIT_FAILURE);
    }

    //close write end of pipe
    close(pipefd[1]);
    waitpid(pid, NULL, 0);

    //reads output from pipe
    len = read(pipefd[0], buffer, CMD_MAX - 1);
    if (len >= 0) {
        buffer[len] = '\0';
        *output = strdup(buffer);
    } else {
        perror("read");
        return ERR_MEMORY;
    }

    close(pipefd[0]);

    //strip all whitespace from captured output
    char *stripped_output = malloc(strlen(*output) + 1);
    char *ptr = stripped_output;

    for (int i = 0; (*output)[i]; i++) {
        if (!isspace((*output)[i])) {
            //copys non-whitespace characters
            *ptr++ = (*output)[i];
        }
    }
    *ptr = '\0';

    free(*output);
    *output = stripped_output;

    return OK;
}

int execute_pipeline(command_list_t *clist) {
    int num_cmds = clist->num;
    int pipes[num_cmds - 1][2];
    pid_t pids[num_cmds];
    char *output = NULL;

    for (int i = 0; i < num_cmds; i++) {
        if (i < num_cmds - 1 && pipe(pipes[i]) < 0) {
            perror("pipe");
            return ERR_MEMORY;
        }

        pids[i] = fork();
        if (pids[i] < 0) {
            perror("fork");
            return ERR_MEMORY;
        } else if (pids[i] == 0) {
            //child process
            if (i > 0) {
                dup2(pipes[i - 1][0], STDIN_FILENO);
                close(pipes[i - 1][0]);
                close(pipes[i - 1][1]);
            }
            if (i < num_cmds - 1) {
                dup2(pipes[i][1], STDOUT_FILENO);
                close(pipes[i][0]);
                close(pipes[i][1]);
            }
            execvp(clist->commands[i].argv[0], clist->commands[i].argv);
            perror("execvp");
            exit(EXIT_FAILURE);
        }

        if (i > 0) {
            close(pipes[i - 1][0]);
            close(pipes[i - 1][1]);
        }
    }

    //captures the output of the last command in the pipeline
    if (num_cmds > 0) {
        exec_cmd_and_capture_output(&clist->commands[num_cmds - 1], &output);
    }

    //just some clean up
    for (int i = 0; i < num_cmds; i++) {
        waitpid(pids[i], NULL, 0);
    }

    //finalize output by stripping any remaining whitespace
    if (output) {
        //strips remaining whitespace from the output
        char *final_output = malloc(strlen(output) + 1);
        char *ptr = final_output;

        for (int i = 0; output[i]; i++) {
            if (!isspace(output[i])) {
                *ptr++ = output[i];
            }
        }
        *ptr = '\0';

        free(output);
        output = final_output;

        //should print the final stripped output on a single line
        printf("%s\n", output);
    }
    return OK;
}

int exec_local_cmd_loop() {
    char *cmd_buff = malloc(SH_CMD_MAX);
    if (!cmd_buff) {
        perror("malloc");
        return ERR_MEMORY;
    }

    command_list_t cmd_list;
    int rc = OK;

    while (1) {
        printf("%s", SH_PROMPT);
        if (fgets(cmd_buff, SH_CMD_MAX, stdin) == NULL) {
            printf("\n");
            break;
        }

        //removes trailing newline
        cmd_buff[strcspn(cmd_buff, "\n")] = '\0';

        //build the command list from old hws
        rc = build_cmd_list(cmd_buff, &cmd_list);
        if (rc == WARN_NO_CMDS) {
            printf(CMD_WARN_NO_CMD);
            continue;
        } else if (rc == ERR_TOO_MANY_COMMANDS) {
            printf(CMD_ERR_PIPE_LIMIT, CMD_MAX);
            continue;
        }

        //check for built-in commands
        Built_In_Cmds bi_cmd = match_command(cmd_list.commands[0].argv[0]);
        if (bi_cmd == BI_CMD_EXIT) {
            free_cmd_list(&cmd_list);
            free(cmd_buff);
            return OK_EXIT;
        } else if (bi_cmd != BI_NOT_BI) {
            exec_built_in_cmd(&cmd_list.commands[0]);
            continue;
        }

        //execute commands and handling pipes
        if (cmd_list.num > 1) {
            rc = execute_pipeline(&cmd_list);
        } else {
            rc = exec_cmd(&cmd_list.commands[0]);
        }

        //free command list
        free_cmd_list(&cmd_list);
    }

    free(cmd_buff);
    return rc;
}