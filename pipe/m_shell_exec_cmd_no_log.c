#define _GNU_SOURCE
#include <stdio.h>
#include "m_shell_parse.h"
#include <error.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include "m_shell_log.h"
#include <time.h>
#include <limits.h>
#include <errno.h>

void
cmd_exec_pipe(struct cmd_container *cmd_list_cnt, int logging)
{
    int cmd_count = cmd_list_cnt->cmd_count;
    int pipe_count = 2 * (cmd_count - 1);
    int pipefds[pipe_count];
    struct cmd_entry *cmd = cmd_list_cnt->cmd_list;
    int index = 1;
    int stream_nav = -1;
    char *cmd_names[cmd_count];
    pid_t cmd_pid[cmd_count];
    int int_pipe[2];
    pipe(int_pipe);
    for(int i = 0; i < pipe_count; i += 2)
    {
        if (pipe(pipefds + i) == -1) {
            perror("Pipe couldn't be initialised\n");
            //should you log here?
        }
    }

    int j = 0;
    while (cmd)
    {
        int count = 0;
        /* List the args */
        char *cmd_dup = cmd->cmd;
        char **arg_list = (char **)malloc(20 * sizeof(char *));
        while ((*(arg_list + count) = strsep(&cmd_dup, " \t")) != NULL)
        {
            if (strlen(arg_list[count]) > 0)
            count++;
        }
        *(arg_list + count) = NULL;
        cmd_names[index - 1] = arg_list[0];

        if(index!= 1 && fork() == 0)
        {
            int len, slen;
            int fd = open("output.log", O_RDWR | O_CREAT| O_TRUNC, S_IRUSR | S_IWUSR);
            write(fd, cmd_names[index - 1], strlen(cmd_names[index - 1]));
            close(int_pipe[0]);
            do {
                close(pipefds[j-1]);
                len = tee(pipefds[j-2], int_pipe[1],
                            INT_MAX, SPLICE_F_NONBLOCK);
                if (len < 0) {
                    if (errno == EAGAIN)
                    {
                        printf("again");
                        continue;
                    }
                    perror("hitee");
                    exit(EXIT_FAILURE);
                } else
                    if (len == 0)
                        break;
                    //read(int_pipe, buf, INT_MAX);
                    //write(fd, buf, strlen(buf));
                    while (len > 0) {
                    slen = splice(pipefds[j-2], NULL, fd, NULL,
                                    len, SPLICE_F_MOVE);
                    if (slen < 0) {
                        perror("splice");
                        break;
                    }
                    len -= slen;
                }
            } while (1);
            close(fd);
            close(int_pipe[1]);
            for(int i = 0; i < pipe_count; i++)
            {
                close(pipefds[i]);
            }
            exit(0);
        }


        if ((cmd_pid[index - 1] = fork()) == 0) {
            if (index == 1)
            {
                if (index != cmd_list_cnt->cmd_count)
                {
                    //op_stream = ;
                    dup2(pipefds[j+1], 1);
                    
                }
                close(int_pipe[1]);
                close(int_pipe[0]);
            }
            else if (index != cmd_list_cnt->cmd_count) {
                close(pipefds[j-1]);
                //dup2(pipefds[j-2], 0);
                dup2(int_pipe[0], 0);
                
                close(int_pipe[1]);
                close(int_pipe[0]);
                //op_stream = ;
                dup2(pipefds[j+1], 1);
            }
            else if (index == cmd_list_cnt->cmd_count) {
                close(int_pipe[1]);
                dup2(int_pipe[0], 0);
                //dup2(pipefds[j-2], 0);
                close(int_pipe[0]);
            }

            for(int i = 0; i < pipe_count; i++)
            {
                close(pipefds[i]);
            }
            execvp(arg_list[0], arg_list);
            exit(-1);
        }
        int status;

        index++;
        j+=2;
        cmd = cmd->cmd_next;
    }
    for(int i = 0; i < pipe_count; i++)
    {
        close(pipefds[i]);
    }
    close(int_pipe[1]);
    close(int_pipe[0]);
    pid_t pid;
    int status;
    while((pid = wait(&status)) != -1)
    {
        if (logging)
        {
            time_t cur_time ;
            time(&cur_time);

            for(int i = 0; i < cmd_count; i++)
            {
                if(pid == cmd_pid[i])
                {
                    m_shell_cmd_log(cmd_names[i], cur_time, status);
                    break;
                }
            }
        }
    }
}

int
cd(char *arg)
{
    char **arg_list = (char **)malloc(2 * sizeof(char *));
    char *cmd_dup = arg;
    int count = 0;
    while ((*(arg_list + count) = strsep(&cmd_dup, " \t")) != NULL)
    {
        if (strlen(arg_list[count]) > 0)
        count++;
    }

	if (arg_list[1] == NULL)
	{
        fprintf(stderr, "No args for cd\n");
        return 1;
    }
	else if (chdir(arg_list[1]) != 0)
	{
        perror("chdir failed");
        return 1;
    }
	return 0;
}

