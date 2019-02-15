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
cmd_exec(struct cmd_container *cmd_list_cnt)
{
    int pipe_count = cmd_list_cnt->cmd_count;

    struct cmd_entry *cmd = cmd_list_cnt->cmd_list;
    int index = 1;
    int stream_nav = -1;
    //printf("count:%d", cmd_list_cnt->cmd_count);
    while (cmd)
    {
        stream_nav *= -1;
        int count = 0;
        /* List the args */
        char *cmd_dup = cmd->cmd;
        char **arg_list = (char **)malloc(20 * sizeof(char *));
        while ((*(arg_list + count) = strsep(&cmd_dup, " ")) != NULL)
        {
            if (strlen(arg_list[count]) > 0)
            count++;
        }
        *(arg_list + count) = NULL;

        if (!fork()) {
            if(index == 1)
            {
                int op_stream = open("output.txt", O_RDWR | O_CREAT| O_TRUNC, S_IRUSR | S_IWUSR);
                dup2(op_stream, 1);
                close(op_stream);
            }
            else if(index == cmd_list_cnt->cmd_count) {
                if (stream_nav == -1)
                {
                    int op_stream = open("output.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
                    dup2(op_stream, 0);
                    close(op_stream);
                }
                else
                {
                    int ip_stream = open("input.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
                    dup2(ip_stream, 0);
                    close(ip_stream);
                }
            }
            else  if (stream_nav == -1){
                int ip_stream = open("input.txt", O_RDWR | O_CREAT| O_TRUNC, S_IRUSR | S_IWUSR);
                int op_stream = open("output.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
                dup2(op_stream, 0);
                dup2(ip_stream, 1);
                close(ip_stream);
                close(op_stream);
            }
            else {
                int ip_stream = open("input.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
                int op_stream = open("output.txt", O_RDWR | O_CREAT| O_TRUNC, S_IRUSR | S_IWUSR);
                dup2(ip_stream, 0);
                dup2(op_stream, 1);
                close(ip_stream);
                close(op_stream);
            }

           // printf("indeex %d arg0 is%s\n", index, arg_list[0]);
            execvp(arg_list[0], arg_list);
            exit(-1);
            }
        int status;
        wait(&status);
        //log this
        time_t cur_time ;
        time(&cur_time);
        //printf("%d", status);
        m_shell_cmd_log(arg_list[0], cur_time, status);
        index++;
        cmd = cmd->cmd_next;
    }
}

#define bsz 80
int main()
{
    char *cmd = (char*)malloc(bsz * sizeof(char*));
    strcpy(cmd,"ls -lha -r| grep m_shell|sort |wc");
    //strcpy(cmd," grep mollusc grep-test.c");
    struct cmd_container *cmd_list_cnt = cmd_tokenize(cmd);
    //cmd_container_print(cmd_list_cnt);
    cmd_exec(cmd_list_cnt);
return 0;
}