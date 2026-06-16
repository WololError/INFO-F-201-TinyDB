#include "command.h"

command_t get_command_type(const char *line) {
    char command[16];

    if (sscanf(line, "%15s", command) != 1) {
        return CMD_UNKNOWN;
    }

    if (strcmp(command, "select") == 0) {
        return CMD_SELECT;
    }

    if (strcmp(command, "insert") == 0) {
        return CMD_INSERT;
    }

    if (strcmp(command, "delete") == 0) {
        return CMD_DELETE;
    }

    if (strcmp(command, "update") == 0) {
        return CMD_UPDATE;
    }

    return CMD_UNKNOWN;
}
