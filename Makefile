all:
	gcc -o mollusc m_shell_main.c m_shell_setup.c
clean:
	rm -rf mollusc
