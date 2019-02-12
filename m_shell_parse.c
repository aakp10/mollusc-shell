#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/**
* Change the allocation to bufsize
* Support Reallocation.
*/
char ***
tokenize_cmd(char *cmd)
{
    int count = 0;
    char **cmd_list = (char **) malloc(4*sizeof(char *));
    char ***cmd_index = (char ***) malloc(4*sizeof(char *));
    while((*(cmd_list + count) = strsep(&cmd, "|")) != NULL)
    {
        count++;
    }
    for(int i = 0; i < count; i++)
    {
        int args = 0;
        char **ptr = (char **)malloc(10 * sizeof(char *));
        while((ptr[args] = strsep(&cmd_list[i], " \t")) != NULL)
        {
            if (strlen(ptr[args]) > 0)
                args++;
        }
        //free cmd_ptr
        cmd_index[i] = ptr;
        printf("\n%d: %s", i+1);
        for(int j = 0; j < args; j++)
        {
            printf("%s,", cmd_index[i][j]);
        }
    }
    //free ptrs in cmd_index.
}