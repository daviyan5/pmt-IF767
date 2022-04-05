#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "alg_utils_sg.hpp"
#include "pre_processing.hpp"
#include "wumanber.hpp"

//TO DO
//1. terminar de comentar o codigo
//2. relatorio

alg_print_ret_sg wumanber(char *text, char *patt, int patt_size, int text_size, int max_count, int distance, bool ignore_case, long long *C){
    alg_print_ret_sg ret;

    ret.num_occ = 0;
    ret.occ = (int*)malloc(1 * sizeof(int));

    long long *S =  (long long*) malloc((dist + 1) * sizeof(long long));
    S[0] = (1 << patt_size) - 1;

    for(int q = 1; q <= distance; q++){
        S[q] = (S[q - 1] << 1);
    }

    long long *S_aux;
    S_aux = (long long*)malloc((distance + 1)*sizeof(long long));
    
    for(int i = 0; i < text_size; i++){
        S_aux[0] =  ((S[0] << 1) | C[text[i]]);
        for(int q = 1; q <= distance; q++){
            int letter = isLatin(text[i]);
            if(ignore_case && letter != -1) {
                S_aux[q] = ((S[q] << 1) | C[letter]) & (S[q - 1] << 1) 
                          & (S[q - 1] << 1) & (S_aux[q - 1] << 1) & S[q - 1];
            }
            else S_aux[q] = ((S[q] << 1) | C[text[i]]) & (S[q - 1] << 1) & (S_aux[q - 1] << 1) & S[q - 1];
        }

        for(int j = 0; j <= distance; j++) S[j] = S_aux[j];

        if((S[distance] & (1 << (patt_size - 1))) == 0){
            ret.num_occ++;
            ret.occ = (int *)realloc(ret.occ, ret.num_occ * sizeof(int));
            ret.occ[ret.num_occ - 1] = i - patt_size + 1;
        }
    }
    
    free(S);
    free(S_aux);

    return ret;
}

//non