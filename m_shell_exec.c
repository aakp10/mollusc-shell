#include <stdio.h>
#include "m_shell_parse.h"
#include <error.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

void cmd_exec(struct cmd_container *cmd_list_cnt)
{
    int pipe_count = cmd_list_cnt->cmd_count;
    /*int pipefds[2*pipe_count];
    for (int i = 0; i < 2*pipe_count; i+=2)
    {
        if (pipe(pipefds + i) < 0) {
            perror("Pipe couldn't be initialized");
            exit(1);
        }
    }*/

    struct cmd_entry *cmd = cmd_list_cnt->cmd_list;
    int index = 1;
    printf("count:%d", cmd_list_cnt->cmd_count);
    while (cmd)
    {
        //printf("CMD %s", cmd->cmd);
        //printf("executed cmd %d", index);

        int ip_stream = open("input.txt", O_RDWR|O_CREAT);
        int op_stream = open("output.txt", O_RDWR|O_CREAT);
        if (!fork()) {
            if(index != 1)
            {
                dup2(op_stream, 0);
            }
                
            if(index != cmd_list_cnt->cmd_count)
                dup2(op_stream, 1);
            close(op_stream);
            close(ip_stream);
            int count = 0;
            /* List the args */
            char *cmd_dup = cmd->cmd;
            char *curr_arg;
            char **arg_list = (char **)malloc(20 * sizeof(char *));
            while ((*(arg_list + count) = strsep(&cmd_dup, " ")) != NULL)
            {
                if (strlen(arg_list[count]) > 0)
                count++;
            }

            *(arg_list + count) = NULL;
           // printf("indeex %d arg0 is%s\n", index, arg_list[0]);
            execvp(arg_list[0], arg_list);
            exit(0);
            }
        wait(NULL);
        index++;
        cmd = cmd->cmd_next;
            close(op_stream);
    close(ip_stream);
    }
}

#define bsz 80
int main()
{
    char *cmd = (char*)malloc(bsz * sizeof(char*));
    strcpy(cmd,"ls -lha -r| wc");
    //strcpy(cmd," grep mollusc grep-test.c");
    struct cmd_container *cmd_list_cnt = cmd_tokenize(cmd);
    //cmd_container_print(cmd_list_cnt);
    cmd_exec(cmd_list_cnt);
    return 0;
}