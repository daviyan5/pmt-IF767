#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "alg_utils_sg.hpp"
#include "alg_template.hpp"

using namespace std;

bool isEqual(char a, char b, bool ignore_case){
    if(ignore_case){
        a = tolower(a);
        b = tolower(b);
    }
    return a == b;
}


int* init_strict_next(char *pat, int patt_size, bool ignore_case){
    int m = patt_size;
    int *nxt = (int*) malloc((m+1) * sizeof(int));
    memset(nxt, -1, sizeof(nxt));

    if(m == 1 || (m > 1 && !isEqual(pat[0],pat[1],ignore_case))) nxt[1] = 0;

    int i = 1, j = 0;
    while(i + j < m){
        while(i + j < m and isEqual(pat[i+j], pat[j],ignore_case)){
            j++;
            if(i + j == m || !isEqual(pat[i+j], pat[j],ignore_case)) nxt[i + j] = j;
            else nxt[i + j] = nxt[j];
        }

        if(j == 0 && ((i + 1 == m) || (!isEqual(pat[0], pat[i + 1], ignore_case)))){
            nxt[i + 1] = 0;
        }

        i += (j - nxt[j]);
        j = max(0, nxt[j]);
        
    }

    return nxt;
}

alg_print_ret_sg kmp(char *txt, char *pat, int patt_size, int text_size, int max_count, bool ignore_case){
    
    alg_print_ret_sg ret;
    ret.num_occ = 0;                                
    ret.occ = (int *) malloc(1 * sizeof(int)); 

    if(max_count == 0) return ret;

    int occ_sz = 1;
    int n = text_size;
    int m = patt_size;
    int *nxt = init_strict_next(pat, patt_size, ignore_case);   
    int i = 0, j = 0;
    while(i <= n - m){

        while(j < m and isEqual(txt[i+j], pat[j], ignore_case)){
            j++;
        }
        if(j == m){
            if(ret.num_occ <= occ_sz - 1){
                ret.occ[ret.num_occ] = i;
                ret.num_occ++;
            }
            else{
                occ_sz *= 2;
                ret.occ = (int *) realloc(ret.occ, occ_sz * sizeof(int));
                ret.occ[ret.num_occ] = i;
                ret.num_occ++;
            }
            if(ret.num_occ == max_count) return ret;
        }
        i += (j - nxt[j]);

        j = max(0, nxt[j]);
    }

    return ret;
}