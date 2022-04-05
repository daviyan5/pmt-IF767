#ifndef H_ALG_BOYER_MOORE
#define H_ALG_BOYER_MOORE
#include "algorithms.hpp"

alg_print_ret_sg boyermoore(char *txt, char *patt, int patt_size,int text_size, int max_count,bool ignore_case,int *bc, int *gs);

#endif 