#ifndef H_AHO_CORASICK
#define H_AHO_CORASICK
#include "algorithms.hpp"

alg_print_ret_mul aho_corasick(char *text, int *patt_size, int num_patt, int text_size, bool ignore_case,
                               int **go_to,int *fail, int *qnt_occ, int **occ);

#endif 