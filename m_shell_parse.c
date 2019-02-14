#include <stdio.h>
#include <stdlib.h>
#include "m_shell_parse.h"

static struct cmd_entry*
new_cmd_entry(char *cmd_ptr, struct cmd_entry *next_cmd_entry)
{
    struct cmd_entry *temp = (struct cmd_entry*) malloc(sizeof(temp));
    temp->cmd = cmd_ptr;
    temp->cmd_next = NULL;
    if (next_cmd_entry != NULL) {
        next_cmd_entry->cmd_next = temp;
    }
    return temp;
}

static struct cmd_cointainer*
new_cmd_container()
{
    struct cmd_cointainer *temp = (struct cmd_cointainer*) malloc(sizeof(temp));
    temp->cmd_list = NULL;
    temp->cmd_tail = NULL;
    temp->cmd_count = 0;
    return temp;
}

static void
cmd_cointainer_insert(struct cmd_cointainer *cmd_list_cnt, char *cmd_ptr)
{
    struct cmd_entry *temp_cmd_entry = new_cmd_entry(cmd_ptr, cmd_list_cnt->cmd_tail);
    if (cmd_list_cnt->cmd_list == NULL)
        cmd_list_cnt->cmd_list = temp_cmd_entry;
    cmd_list_cnt->cmd_tail = temp_cmd_entry;
    cmd_list_cnt->cmd_count += 1;
}

static void
cmd_cointainer_print(struct cmd_cointainer *cmd_list_cnt)
{
    struct cmd_entry *iter = cmd_list_cnt->cmd_list;
    int index = 1;
    while(iter)
    {
        printf("%d :%s\n", index++, iter->cmd);
        iter = iter->cmd_next;
    }
}

// FREE : after executing the cmds mentioned in the container.
struct cmd_cointainer*
cmd_tokenize(char *cmd)
{
    int count = 0;
    char *curr_cmd;
    char *cmd_dup = cmd;
    //FREE cmd_list if count == 0 ; But that won't be a case ;)
    struct cmd_cointainer *cmd_list = new_cmd_container();
    while((curr_cmd = strsep(&cmd_dup, "|")) != NULL)
    {
        count++;
        //insert this cmd into the container
        cmd_cointainer_insert(cmd_list, curr_cmd);
    }
    return cmd_list;
}