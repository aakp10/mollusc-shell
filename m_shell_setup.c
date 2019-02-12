#include "m_shell_setup.h"
#include <stdlib.h>

m_shell_state*
m_shell_init_state(void)
{
    m_shell_state *new_shell_state =  (m_shell_state*) malloc(sizeof(m_shell_state));
    new_shell_state->running_state = UNCERTAIN;
    return new_shell_state;
}

void
m_shell_change_state(m_shell_state *shell_state, enum state new_state)
{
    shell_state->running_state = new_state;
}

int
m_shell_is_exit(m_shell_state *shell_state)
{
    if (shell_state->running_state == UNCERTAIN || shell_state->running_state == EXIT)
        return 1;
    return 0;
}

enum state
m_shell_get_state(m_shell_state *shell_state)
{
    return shell_state->running_state;
}