#ifndef _COMMAND_H
#define _COMMAND_H

#include <string.h>
#include <stdio.h>

typedef enum {
    CMD_SELECT,
    CMD_INSERT,
    CMD_DELETE,
    CMD_UPDATE,
    CMD_UNKNOWN
} command_t;


command_t get_command_type(const char *line);



#endif