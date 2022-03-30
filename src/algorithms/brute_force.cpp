#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "pthread.h"
#include "brute_force.hpp"
#include "algorithms.hpp"
#include "time.h"

void *bruteforce(void *args){
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
             
    for(int i = 0; i < n; i++){
        bool foi = true;
        for(int j = 0; j < m; j++){
            if( i + j >= n or params->txt[i+j] != params->patt[j]){
                foi = false;
                break;
            }
        }
        if(foi){
            ret->occ[ret->num] = i;
            ret->num ++;
            if(ret->num >= size){
                size *= 2;
                ret->occ = (int *) realloc(ret->occ,size * sizeof(int));
            } 
        }
    }

    end = clock();
    ret->time = (double) (end - start) / CLOCKS_PER_SEC;
    pthread_exit(ret);
}