#include "parser.h"

#include <stddef.h>
#include <stdbool.h>
#include <string.h>

// Note: maybe use https://github.com/ryanflannery/str2argv
//       doesn't seem to support positional argument decision trees though


size_t parse_cmd(char *cmd, size_t cmd_size, char **arg_list, size_t arg_max)
{
    size_t arg_count = 0;
    char *cur_arg = cmd;

    // Windows sends \r\n instead of \n
    // CMD sends the space before the pipe character, so we need to escape that too.
    //
    // POWERSHELL USERS: You need to use `echo "screen on"` instead of `echo screen on`
    // Otherwise Powershell will send \r\n as a word separator, ugh.
    //
    //
    // Yes, this is a hack, yes, I could probably do it in a better way, maybe later.
    for (size_t i = 0; i < cmd_size; i++)
    {
        if (cmd[i] == '\r')
        {
            if (cmd[i - 1] == ' ') // CMD
            {
                cmd[i - 1] = '\n';
                cmd[i] = '\0';
                cmd[i + 1] = '\0';
                cmd_size -= 2;
            }
            else // Powershell
            {
                cmd[i] = '\n';
                cmd[i + 1] = '\0';
                cmd_size--;
            }
            break;
        }
    }

    // load my stuff\n ==> load\0my\0stuff\0
    for (size_t i = 0; (i < cmd_size) && (arg_count < arg_max); i++)
    {
        if (cmd[i] == ' ' || cmd[i] == '\n')
        {
            cmd[i] = '\0';
            arg_list[arg_count] = cur_arg;

            cur_arg = &(cmd[i+1]);
            arg_count += 1;
        }

        if (cmd[i] == '\n')
            break;
    }

    return arg_count;
}