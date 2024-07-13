//
// Created by arthur on 13/06/24.
//
#include <request.h>

int32_t send_request(int fd, const std::vector<std::string> &command)
{
    uint32_t len = 4;
    for (const std::string &s : command)
    {
        len += 4 + s.size();
    }
    if (len > k_max_msg)
    {
        return -1;
    }
    char wbuff[4 + k_max_msg];
    memcpy(wbuff, &len, 4); // here I'm assuming little endian
    uint32_t n = command.size();
    memcpy(&wbuff[4], &n, len);
    size_t cur = 8;
    for (const std::string &s : command)
    {
        uint32_t p = (uint32_t)s.size();
        memcpy(&wbuff[cur], &p, 4);
        memcpy(&wbuff[cur + 4], s.data(), s.size());
        cur += 4 + s.size();
    }
    return write_all(fd, wbuff, 4 + len);
}