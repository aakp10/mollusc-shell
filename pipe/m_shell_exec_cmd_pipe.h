#ifndef __M_SHELL_EXEC_CMD_PIPE_H_
#define __M_SHELL_EXEC_CMD_PIPE_H_

#include "m_shell_parse.h"
void    cmd_exec_pipe(struct cmd_container *cmd_list_cnt, int logging);
void    cd(char *arg);
#endif