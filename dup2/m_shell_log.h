#ifndef __M_SHELL_LOG_H_
#define __M_SHELL_LOG_H_

#include <time.h>

void    m_shell_cmd_log(char *, time_t, int);
void    m_shell_op_log(char *, char *);
void    m_shell_viewcmdlog();
void    m_shell_viewoutlog();
#endif