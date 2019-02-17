#include <stdio.h>
#include <string.h>
#include "m_shell_setup.h"
#include "m_shell_exec_cmd_pipe.h"
#include "m_shell_log.h"

#define MAX_LEN 80
int logging = 0;
int main(int argc, char *argv[])
{
    char *cmd = NULL;
    size_t linecap;
    m_shell_state *shell_state = m_shell_init_state();
    while(1)
    {
        cmd = NULL;
        linecap = MAX_LEN;
        printf("> ");
        int len_read = getline(&cmd, &linecap, stdin);
        while(strlen(cmd) == 1) // only \n
        {
            printf("> ");
            len_read = getline(&cmd, &linecap, stdin);
        }
        if (strncmp(cmd, "entry", strlen("entry")) == 0) {
            if (m_shell_get_state(shell_state) != ENTRY)
                m_shell_change_state(shell_state, ENTRY);
            else
                fprintf(stdout, "? Session in progress...\n");
        }
        else if (strncmp(cmd, "exit", strlen("exit")) == 0) {
            if (m_shell_is_exit(shell_state)) {
                m_shell_change_state(shell_state, UNCERTAIN);
                fprintf(stdout, "? Exiting...\n");
                return 0;
            }
            m_shell_change_state(shell_state, EXIT);
        }
        else if (m_shell_get_state(shell_state) == ENTRY) {
            /* cmd to process */
            /*WIP*/
            //log
            if (strncmp(cmd, "log", strlen("log")) == 0) {
                logging = 1;
            }
            //unlog
            else if (strncmp(cmd, "unlog", strlen("unlog")) == 0) {
                logging = 0;
            }
            //viewcmdlog
            else if (strncmp(cmd, "viewcmdlog", strlen("viewcmdlog")) == 0) {
                m_shell_viewcmdlog();
            }
            //viewoplog
            else if (strncmp(cmd, "viewoutlog", strlen("viewoutlog")) == 0) {
                m_shell_viewoutlog();
            }
            //cd
            else if (strncmp(cmd, "changedir", strlen("changedir")) == 0) {
                cmd[strlen(cmd) - 1] = '\0';
                cd(cmd);
            }
            else{
            //external
            printf("%s", cmd);
            cmd[strlen(cmd)-1] = '\0';
            struct cmd_container *cmd_list_cnt = cmd_tokenize(cmd);
            cmd_container_print(cmd_list_cnt);
            printf("will be exec\n");
            cmd_exec_pipe(cmd_list_cnt, logging);
            }
        }
        else {
            fprintf(stdout, "? Enter a new session to continue\n");
        }
    }
    return 0;
}