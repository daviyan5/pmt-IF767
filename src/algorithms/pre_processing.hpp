#ifndef H_PRE_PROCESS
#define H_PRE_PROCESS
#include "algorithms.hpp"


extern int **strict_nxt, **bad_char, **good_suffix;
extern int **aho_occ, *aho_qnt_occ;
extern int **aho_go_to, *aho_fail;
extern int size_goto, occ_size;
extern long long **C_wu;

// Tamanho do alfabeto considerado (128 padrão)
const int alpha_len = 128;

int isLatin(char c);
bool isEqual(char a, char b, bool ignore_case);
void preprocess(Args &pmt,vector<vector<int>> &alg_used);

#endif 