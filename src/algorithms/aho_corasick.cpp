#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "alg_utils_sg.hpp"
#include "pre_processing.hpp"
#include "aho_corasick.hpp"


// Algoritmo Aho - Corasick
alg_print_ret_mul aho_corasick(char *text, int *patt_size, int num_patt, int text_size, bool ignore_case,
                               int **go_to,int *fail, int *qnt_occ, int **occ){
    alg_print_ret_mul ret;
    ret.num_occ = (int*) calloc((num_patt+1) ,sizeof(int));
    ret.occ = (int**) calloc((num_patt+1), sizeof(int*));
    ret.total_occ = 0;
    int n = text_size;
    int node = 0, i = 0;

    while(i < n){
        int ch = ignore_case? tolower(text[i]):text[i];
        while(go_to[node][ch] < 0){
            node = fail[node];
        }
        node = go_to[node][ch];
        for(int j = 0; j < qnt_occ[node]; j++){
            int k = occ[node][j];
            ret.total_occ++;
            ret.num_occ[k]++;
            ret.occ[k] = (int*) realloc(ret.occ[k], ret.num_occ[k] * sizeof(int));
            ret.occ[k][ret.num_occ[k] - 1] = i - patt_size[k] + 1;
        }
        i++;
    }
    
    return ret;
}

