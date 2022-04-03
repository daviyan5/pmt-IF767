#ifndef H_KMP
#define H_KMP
#include "algorithms.hpp"

alg_print_ret_sg kmp(char *text, char *patt, int patt_size,int text_size, int max_count,bool ignore_case);

#endif 