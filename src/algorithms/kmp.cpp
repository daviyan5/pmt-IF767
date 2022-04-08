#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "alg_utils_sg.hpp"
#include "pre_processing.hpp"
#include "kmp.hpp"

using namespace std;

alg_print_ret_sg kmp(char *txt, char *pat, int patt_size, int text_size, int max_count, bool ignore_case, int *nxt){
    alg_print_ret_sg ret;
    ret.num_occ = 0;                                
    ret.occ = (int *) malloc(1 * sizeof(int)); 
    
    if(max_count == 0) return ret;

    int occ_sz = 1;
    int n = text_size;
    int m = patt_size;
    
    int j = 0;
    for(int i = 0; i <= n-m;){
        while(j < m and isEqual(txt[i+j], pat[j], ignore_case)) j++;
        if(j == m){
            if(ret.num_occ < occ_sz){
                ret.occ[ret.num_occ] = i;
                ret.num_occ++;
            }
            else{
                ret.num_occ++;
                ret.occ = (int *) realloc(ret.occ, ret.num_occ * sizeof(int));
                //occ_sz *= 2;
                //ret.occ = (int *) realloc(ret.occ, occ_sz * sizeof(int));
                ret.occ[ret.num_occ - 1] = i;
            }
            if(ret.num_occ == max_count) return ret;
        }
        i += (j - nxt[j]);
        j = max(0, nxt[j]);
    }

    return ret;
}