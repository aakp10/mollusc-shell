# CMD execution using pipes

Usual stuff
* `make`

* `./mollusc-pipe`

# All my sins:

* The file [m_shell_exec_cmd_no_log.c](https://github.com/aakp10/mollusc-shell/blob/master/pipe/m_shell_exec_cmd_no_log.c) is ideally expected to execute any usual shell cmd.  

    pipefds is used to redirect the stdout of the process.

    Since, I'm required to log intermediate cmd o/p, a new process handling `tee` operation is created.

    It uses the int_pipe to redirect the stdout's content to the stdin.
    It turns out that at times(cmds w/ two pipes) it's returning `EAGAIN` error. I suppose this would be because the reader isn't running and hence the pipe buffer is full but at the same time I've made sure that no two related procs die.