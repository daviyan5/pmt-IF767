#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "pthread.h"
//#include "alg_name.hpp"
#include "algorithms.hpp"
#include "time.h"

void *alg_name(void *args){
    alg_params_sg *params;
    params = (alg_params_sg*) args;
    alg_ret_sg *ret;
    ret = new alg_ret_sg();
    clock_t start, end;
    int size = 1;
    ret->num = 0;
    ret->occ = (int *) malloc(size * sizeof(int));
    start = clock();
    int n = strlen(params->txt);
    int m = strlen(params->patt);        
             
    /*

            ALGORITMO AQUI


    */


    end = clock();
    free(params);
    ret->time = (double) (end - start) / CLOCKS_PER_SEC;
    pthread_exit(ret);
}