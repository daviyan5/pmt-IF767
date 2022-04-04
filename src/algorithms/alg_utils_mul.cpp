#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "pthread.h"
#include "time.h"

#include "aho_corasick.hpp"

#include "algorithms.hpp"
#include "pre_processing.hpp"


#define TEXT_MAX_SIZE 1000000

void print_occ_mul(char *text, int **occ, int *num_occ, int total_occ, int text_size,int num_patt, int line_number, int *patt_size, 
               char *file_name,FILE *out_file)
{
    blue();
    if(is_out_file) fprintf(out_file,"\n%d Occurrences in Line %d of %s\n",total_occ, line_number, file_name);
    else printf("\n%d Occurrences in Line %d of %s\n",total_occ, line_number, file_name);
    default_colour();
    bool *paint = (bool*) calloc(text_size,sizeof(bool));
    int *counter = (int*) calloc(num_patt,sizeof(int));
    for(int i = 0; i < text_size; i++){
        for(int j = 0; j < num_patt; j++){
            if(counter[j] < num_occ[j] and occ[j][counter[j]] == i){
                int pos = occ[j][counter[j]];
                counter[j]++;
                for(int k = pos; k < patt_size[j]; k++) paint[k] = true;
            }
        }
        if(paint[i]) red();
        else default_colour();
        if(is_out_file) fprintf(out_file,"%c",text[i]);
        else printf("%c",text[i]);
        
    }
    default_colour();
    printf("\n");
}

alg_print_ret_mul send_to_func_mul(alg_params_mul *params,char *text,int text_size,int *patt_size){
    switch(params->func){
        case ALG_AHO_CORASICK:
            return aho_corasick(text,patt_size,params->num_patt,text_size,ignore_case,aho_go_to,aho_fail,aho_qnt_occ,aho_occ);
            break;
        case ALG_WU_MANBER:
            break;
    }
    return aho_corasick(text,patt_size,params->num_patt,text_size,ignore_case,aho_go_to,aho_fail,aho_qnt_occ,aho_occ);
}

void *prepare_mul_func(void *args){
    alg_params_mul *params;
    params = (alg_params_mul*) args;

    clock_t start, end;
    int *num_occ = (int*) calloc(params->num_patt,sizeof(int));
    
    char text[TEXT_MAX_SIZE];
    int *patt_size = params->patt_size;
    int line_number = 0;
    FILE *txt_file = fopen(params->file_name,"r");
    FILE *out_file;
    if(is_out_file){
        out_file = fopen(params->out_file,"w");
    }
    int total_occ = 0;
    start = clock();
    while(fgets(text,TEXT_MAX_SIZE,txt_file)){
        int text_size = strlen(text);
        alg_print_ret_mul temp = send_to_func_mul(params,text,text_size,patt_size);
        bool some_occ = false;
        int local_occ = 0;
        for(int i = 0; i < params->num_patt; i++){
            num_occ[i] += temp.num_occ[i];
            local_occ += num_occ[i];
            if(temp.num_occ[i] > 0) some_occ = true;
        }
        if(some_occ > 0 and !only_count){
            pthread_mutex_lock(&global_mutex);
            print_occ_mul(text,temp.occ,temp.num_occ,total_occ - local_occ,text_size,params->num_patt,line_number,patt_size,params->file_name,out_file);
            pthread_mutex_unlock(&global_mutex);
        }
        line_number += 1;
    }
    end = clock();
    double total_time = (double) (end - start) / CLOCKS_PER_SEC;
    if(only_count){
        pthread_mutex_lock(&global_mutex);
        if(is_out_file) fprintf(out_file,"\nIn %s: %d total occurrences.\n",params->file_name,total_occ);
        else printf("\nIn %s: %d total occurrences.\n",params->file_name,total_occ);
        pthread_mutex_unlock(&global_mutex);
    }
    if(show_stt){
        pthread_mutex_lock(&global_mutex);
        if(is_out_file) fprintf(out_file,"Spent time: %lf",total_time);
        else printf("Spent time: %lf",total_time);
        pthread_mutex_unlock(&global_mutex);
    }
    fclose(txt_file);
    if(is_out_file) fclose(out_file);
    pthread_exit(NULL);
}