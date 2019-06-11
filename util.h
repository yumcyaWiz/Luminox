#ifndef UTIL_H
#define UTIL_H
#include <string>
std::string percentage(float x, float max) {
    return std::to_string(x/max*100) + "%";
}
std::string progressbar(float x, float max) {
    const int max_count = 40;
    int cur_count = (int)(x/max * max_count);
    std::string str;
    str += "[";
    for(int i = 0; i < cur_count; i++)
        str += "#";
    for(int i = 0; i < (max_count - cur_count - 1); i++)
        str += " ";
    str += "]";
    return str;
}
#endif
