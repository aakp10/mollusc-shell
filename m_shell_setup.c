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
m_shell_is_valid_state_change(m_shell_state *shell_state, enum state new_state)
{
    
    if(shell_state->running_state == UNCERTAIN) {
        /**
         * UNCERTAIN TO ENTRY => User wants to log in
         */
        if (new_state == ENTRY)
            return 1;
        /**
         * UNCERTAIN TO EXIT => User hasn't logged in
         */
        if (new_state == EXIT)
            return 0;
    }
}