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
    printf("count:%d", cmd_list_cnt->cmd_count);
    for(int i = 0; i < pipe_count; i += 2)
    {
        if (pipe(pipefds + i) == -1) {
            perror("Pipe couldn't be initialised\n");
            //should you log here?
        }
    }
    //int in_stream = dup(0);
    //int stout_dup = dup(1);
    int out_stream;
    /**
    * Setup I/O Redirection
    */

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
        if ((cmd_pid[index - 1] = fork()) == 0) {
            if (index == 1)
            {
                if (index != cmd_list_cnt->cmd_count)
                {
                    dup2(pipefds[j+1], 1);
                }
            }
            else if (index != cmd_list_cnt->cmd_count) {
                dup2(pipefds[j-2], 0);
                dup2(pipefds[j+1], 1);
            }
            else if (index == cmd_list_cnt->cmd_count) {
                dup2(pipefds[j-2], 0);
            }
            
            printf("indeex %d arg0 is%s\n", index, arg_list[0]);
            for(int i = 0; i < pipe_count; i++)
            {
                close(pipefds[i]);
            }
            
            execvp(arg_list[0], arg_list);
            exit(-1);
        }
        int status;
        //wait(&status);
        //log this
        index++;
        j+=2;
        cmd = cmd->cmd_next;
    }
    for(int i = 0; i < pipe_count; i++)
    {
        close(pipefds[i]);
    }
    pid_t pid;
    int status;
    while((pid = wait(&status)) != -1)
    {
        if (logging)
        {
            time_t cur_time ;
            time(&cur_time);
            //printf("%d", status);
            for(int i = 0; i < cmd_count; i++)
            {
                if(pid == cmd_pid[i])
                {
                    m_shell_cmd_log(cmd_names[i], cur_time, status);
                    break;
                }
            }
            
            // USE TEE
            /*char *fnames[] = {"input.txt", "output.txt"};
            if (stream_nav == 1)
                m_shell_op_log(arg_list[0], fnames[1]);
            else
                m_shell_op_log(arg_list[0], fnames[0]);
            */
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

