#ifndef __M_SHELL_SETUP_H_
#define __M_SHELL_SETUP_H_

typedef struct _m_shell_state m_shell_state;
enum state{
    ENTRY,
    EXIT,
    UNCERTAIN,
    NR_STATES
};
struct _m_shell_state{
    enum state running_state;
};

m_shell_state*      m_shell_init_state(void);
void                m_shell_change_state(m_shell_state *, enum state);
//gboolean
int                m_shell_is_valid_state_change(m_shell_state *, enum state);
#endif