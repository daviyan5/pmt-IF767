#ifndef _HAVE_PARSE_H

#define _HAVE_PARSE_H
#include "getopt.h"
enum TYPES{
    TYPE_INFORMATIVE,
    TYPE_IMPERATIVE
};
class Args{
public:
    int type;
    Args(){
        type = -1;
    }
};
Args parse_commands(int argc,char *argv[]);
void read_params(Args &args);
#endif