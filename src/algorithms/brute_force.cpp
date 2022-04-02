#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "alg_utils_sg.hpp"
#include "brute_force.hpp"



alg_print_ret_sg bruteforce(char *text, char *patt, int patt_size,int text_size, int max_count,bool ignore_case){
    alg_print_ret_sg ret;
    ret.num_occ = 0;
    ret.occ = (int *) malloc(1 * sizeof(int));
    int occ_size = 1;
    for(int i = 0; i < text_size; i++){
        bool foi = true;
        for(int j = 0; j < patt_size; j++){
            bool diff = i + j >= text_size? true: ignore_case? tolower(text[i+j]) != tolower(patt[j]) : text[i+j] != patt[j]; 
            if(diff){
                foi = false;
                break;
            }
        }
        if(foi){
            ret.num_occ++;
            ret.occ[ret.num_occ-1] = i;
            if(occ_size == ret.num_occ){
                occ_size *= 2;
                ret.occ = (int*) realloc(ret.occ,occ_size * sizeof(int));
            }
            if(ret.num_occ == max_count) break;
        }
    }
    
    
    return ret;
}
