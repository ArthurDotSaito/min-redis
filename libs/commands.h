//
// Created by arthur on 09/07/24.
//

#ifndef COMMANDS_H
#define COMMANDS_H

#include <common.h>
#include <map>
#include <strings.h>
#include <vector>

enum
{
    RES_OK = 0,
    RES_ERR = 1,
    RES_NX = 2
};

uint32_t handle_get(const std::vector<std::string> &command, uint8_t *res, uint32_t *reslen);
uint32_t handle_set(const std::vector<std::string> &command, uint8_t *res, uint32_t *reslen);
uint32_t handle_delete(const std::vector<std::string> &command, uint8_t *res, uint32_t *reslen);
bool cmd_is(const std::string &word, const char *command);

#endif // COMMANDS_H