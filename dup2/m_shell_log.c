#include "m_shell_log.h"
#include <stdio.h>
#include <fcntl.h>
#include <time.h>

#define CMD_LOG_FNAME "command.log"
#define OP_LOG_FNAME "output.log"

void
m_shell_cmd_log(char *cmd_name, time_t exec_time, int status)
{
    struct tm *cur_time = localtime(&exec_time);
    int hr = cur_time->tm_hour;
    int min = cur_time->tm_min;
    int sec = cur_time->tm_sec;
    int day = cur_time->tm_mday;
    int mth = cur_time->tm_mon + 1;
    int yr = cur_time->tm_year + 1900;
    char *status_msg[] = {"SUCCESS", "FAILURE"};
    status = status == 0? status: 1;
    //cmd_name date time status(0/ !0)
    FILE *fin = fopen(CMD_LOG_FNAME, "a+");
    fprintf(fin, "%s %02d/%02d/%02d %02d:%02d:%02d %s\n",
                cmd_name, day, mth, yr, hr, min, sec, status_msg[status]);
    fclose(fin);
}

void
m_shell_op_log(char *cmd_name, char *op_fname)
{
    char ch;
    FILE *fin = fopen(OP_LOG_FNAME, "a+");
    FILE *fcp = fopen(op_fname, "r");
    fprintf(fin, "\n%s\n", cmd_name);
    while((ch = fgetc(fcp)) != EOF)
    {
        fprintf(fin, "%c", ch);
    }
    fclose(fcp);
    fclose(fin);
}

void
m_shell_viewcmdlog()
{
    char ch;
    FILE *fin = fopen(CMD_LOG_FNAME, "r");
    while((ch = fgetc(fin)) != EOF)
        fprintf(stdout, "%c", ch);
    fclose(fin);
}

void
m_shell_viewoutlog()
{
    char ch;
    FILE *fin = fopen(OP_LOG_FNAME, "r");
    while((ch = fgetc(fin)) != EOF)
        fprintf(stdout, "%c", ch);
    fclose(fin);
}