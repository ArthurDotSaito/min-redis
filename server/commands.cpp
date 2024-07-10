//
// Created by arthur on 09/07/24.
//

#include <commands.h>
#include <string>
#include <vector>

std::map<std::string, std::string> g_map;

uint32_t handle_get(const std::vector<std::string> &command, uint8_t *res, uint32_t *reslen)
{
    if (!g_map.count(command[1]))
    {
        return RES_NX;
    }
    std::string &val = g_map[command[1]];
    assert(val.size() <= k_max_msg);

    memcpy(res, val.data(), val.size());
    *reslen = (uint32_t)val.size();

    return RES_OK;
}

uint32_t handle_set(const std::vector<std::string> &command, uint8_t *res, uint32_t *reslen)
{
    (void)res;
    (void)reslen;
    g_map[command[1]] = command[2];

    return RES_OK;
}

uint32_t handle_delete(const std::vector<std::string> &command, uint8_t *res, uint32_t *reslen)
{
    (void)res;
    (void)reslen;
    g_map.erase(command[1]);

    return RES_OK;
}