all:
	gcc -o mollusc m_shell_main.c m_shell_setup.c m_shell_parse.c m_shell_exec_cmd.c m_shell_log.c
clean:
	rm -rf mollusc
