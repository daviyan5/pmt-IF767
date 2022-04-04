#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "alg_utils_sg.hpp"
#include "alg_template.hpp"

alg_print_ret_sg alg_template(char *text, char *patt, int patt_size,int text_size, int max_count,bool ignore_case,int *nxt){ 
    alg_print_ret_sg ret;
    ret.num_occ = 0;                                
    ret.occ = (int *) malloc(1 * sizeof(int)); 
    if(max_count == 0) return ret;
    int occ_sz = 1;
    int n = text_size;
    int m = patt_size;                              // tamanho de *occ, para realocação dinâmica
    
    return ret;
}