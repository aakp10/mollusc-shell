#ifndef _M_SHELL_PARSE_H_
#define _M_SHELL_PARSE_H_

#define CMD_NR 5
char*** tokenize_cmd(char *cmd);

struct cmd_entry{
    /**
    * array of ptr to the args of the given command
    */
    char* cmd;
    struct cmd_entry* cmd_next;
};

struct cmd_container{
    struct cmd_entry *cmd_list;
    struct cmd_entry *cmd_tail;
    int cmd_count;
};

struct cmd_container*  cmd_tokenize(char *cmd);

#endif