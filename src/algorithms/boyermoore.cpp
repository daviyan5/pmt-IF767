#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "alg_utils_sg.hpp"
#include "boyermoore.hpp"


alg_print_ret_sg boyermoore(char *txt, char *pat, int patt_size,int text_size, int max_count,bool ignore_case,int *bad_char,int *good_suffix){ 
    alg_print_ret_sg ret;
    ret.num_occ = 0;                                
    ret.occ = (int *) malloc(5 * sizeof(int)); 

    if(max_count == 0) return ret;

    int occ_sz = 5;
    int n = text_size;
    int m = patt_size;
    for(int i = 0; i <= n - m;){
        int j = m-1;
        while (j >= 0  and isEqual(txt[i+j], pat[j], ignore_case)) j--;
        if (j < 0){
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
            i += good_suffix[m];
        }
        else{
            i += max(good_suffix[j], j - bad_char[txt[i+j]]);
        }
    }
    
    
    return ret;
}
