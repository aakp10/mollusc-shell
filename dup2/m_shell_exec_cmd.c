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
cmd_exec(struct cmd_container *cmd_list_cnt, int logging)
{
    int pipe_count = cmd_list_cnt->cmd_count;

    struct cmd_entry *cmd = cmd_list_cnt->cmd_list;
    int index = 1;
    int stream_nav = -1;
    //printf("count:%d", cmd_list_cnt->cmd_count);
    char cmd_log_name[100] = "";
    while (cmd)
    {
        stream_nav *= -1;
        int count = 0;
        /* List the args */
        char *cmd_dup = cmd->cmd;
        strcat(cmd_log_name, cmd_dup);
        char **arg_list = (char **)malloc(20 * sizeof(char *));
        while ((*(arg_list + count) = strsep(&cmd_dup, " \t")) != NULL)
        {
            if (strlen(arg_list[count]) > 0)
            count++;
        }
        *(arg_list + count) = NULL;

        if (!fork()) {
           /* if(index == 1 )
            {
                if (index != cmd_list_cnt->cmd_count)
                {
                    int op_stream = open("output.txt", O_RDWR | O_CREAT| O_TRUNC, S_IRUSR | S_IWUSR);
                    dup2(op_stream, 1);
                    close(op_stream);
                }
            }*/
            /*else if(index == cmd_list_cnt->cmd_count) {
                if (stream_nav == -1)
                {
                	int ip_stream = open("input.txt", O_RDWR | O_CREAT| O_TRUNC, S_IRUSR | S_IWUSR);
                    int op_stream = open("output.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
                    dup2(op_stream, 0);
                    dup2(ip_stream, 1);
                    close(op_stream);
                }
                else
                {
                    int ip_stream = open("input.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
                    dup2(ip_stream, 0);
                    close(ip_stream);
                }
            }*/
            if (stream_nav == -1){
                int ip_stream = open("input.txt", O_RDWR | O_CREAT| O_TRUNC, S_IRUSR | S_IWUSR);
                int op_stream = open("output.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
                dup2(op_stream, 0);
                dup2(ip_stream, 1);
                close(ip_stream);
                close(op_stream);
            }
            else if (stream_nav == 1){
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
        if (logging)
        {
            time_t cur_time ;
            time(&cur_time);
            //printf("%s", cmd_log_name);
            m_shell_cmd_log(cmd_log_name, cur_time, status);
            // USE TEE
            char *fnames[] = {"input.txt", "output.txt"};
            if(stream_nav == 1)
            {
                m_shell_op_log(cmd_log_name, fnames[1]);
            }
            else {
                m_shell_op_log(cmd_log_name, fnames[0]);
            }
            
            //
        }
        index++;
        cmd = cmd->cmd_next;
        strcat(cmd_log_name, " | ");
    }
    char *f_name[] = {"input.txt", "output.txt"};
    stream_nav = stream_nav == 1? stream_nav: 0;
	char ch;
	FILE *fin = fopen(f_name[stream_nav], "r");
    //FILE *fcp = fopen("output.log", "a+");
	while((ch = fgetc(fin)) != EOF)
    {
        fprintf(stdout, "%c", ch);
        //fprintf(fcp, "%c", ch);
    }
    //fclose(fcp);
	fclose(fin);
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

/*#define bsz 80
int main()
{
    char *cmd = (char*)malloc(bsz * sizeof(char*));
    //strcpy(cmd,"ls -lha");
    fgets(cmd, 80, stdin);
    printf("%s", cmd);
    cmd[strlen(cmd)-1] = '\0';
    //strcpy(cmd," grep mollusc grep-test.c");
    struct cmd_container *cmd_list_cnt = cmd_tokenize(cmd);
    cmd_container_print(cmd_list_cnt);
    cmd_exec(cmd_list_cnt);
    return 0;
}*/