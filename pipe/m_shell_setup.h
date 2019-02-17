#ifndef __M_SHELL_SETUP_H_
#define __M_SHELL_SETUP_H_

enum state{
    ENTRY,
    EXIT,
    UNCERTAIN,
    NR_STATES
};

typedef struct _m_shell_state m_shell_state;
struct _m_shell_state{
    enum state running_state;
};

m_shell_state*      m_shell_init_state(void);
void                m_shell_change_state(m_shell_state *, enum state);
//gboolean
int                m_shell_is_exit(m_shell_state *);
enum state         m_shell_get_state(m_shell_state *);
#endif