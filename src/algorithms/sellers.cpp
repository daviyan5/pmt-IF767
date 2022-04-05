#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "alg_utils_sg.hpp"
#include "pre_processing.hpp"
#include "sellers.hpp"

using namespace std;

void next_column(int *column, char *patt, int patt_size, char ch, bool ignore_case, int *nxt){
    int j = 0;
    int m = patt_size;
    for(int i = 0; i <= m; i++) nxt[i] = j;

    for(int i = 1; i <= m; i++){
        nxt[i] = min(column[i] + 1, min(nxt[i - 1] + 1, column[i - 1] + (int)(!isEqual(ch, patt[i - 1], ignore_case))));
    }

    for(int i = 0; i <= m; i++) column[i] = nxt[i];
}

alg_print_ret_sg sellers(char *text, char *patt, int patt_size, int text_size, int max_count, int distance, bool ignore_case){
    alg_print_ret_sg ret;

    ret.num_occ = 0;                                
    ret.occ = (int *) malloc(1 * sizeof(int)); 

    int n = text_size;
    int m = patt_size;

    int *column = (int*)malloc((m + 1)*sizeof(int));
    int *nxt = (int*) malloc((m + 1) * sizeof(int));

    for(int i = 0; i <= m; i++) column[i] = i;

    for(int j = 0; j < n; j++){
        next_column(column, patt, patt_size, text[j], ignore_case, nxt);

        if(column[m] <= distance){
            ret.num_occ++;
            ret.occ = (int*)realloc(ret.occ, ret.num_occ*sizeof(int));
            ret.occ[ret.num_occ - 1] = j - patt_size + 1;
        }

        if(ret.num_occ == max_count) return ret;
    }

    free(nxt);
    free(column);

    return ret;
}
