#include <stdio.h>
#include <string.h>
#include "m_shell_setup.h"
#include "m_shell_exec_cmd.h"

#define MAX_LEN 80
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
        if (strncmp(cmd, "entry", len_read - 1) == 0) {
            if (m_shell_get_state(shell_state) != ENTRY)
                m_shell_change_state(shell_state, ENTRY);
            else
                fprintf(stdout, "? Session in progress...\n");
        }
        else if (strncmp(cmd, "exit", len_read - 1) == 0) {
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
            printf("%s", cmd);
            cmd[strlen(cmd)-1] = '\0';
            struct cmd_container *cmd_list_cnt = cmd_tokenize(cmd);
            cmd_container_print(cmd_list_cnt);
            printf("will be exec\n");
            cmd_exec(cmd_list_cnt);
        }
        else {
            fprintf(stdout, "? Enter a new session to continue\n");
        }
    }
    return 0;
}