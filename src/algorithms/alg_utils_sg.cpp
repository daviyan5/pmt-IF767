#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "pthread.h"
#include "time.h"
#include "brute_force.hpp"
#include "algorithms.hpp"


#define TEXT_MAX_SIZE 1000000

void blue(){
    printf("\x1b[34m");
}
void red(){
    printf("\x1b[31m");
}
void default_colour(){
    printf("\x1b[0m");
}
void print_occ(char *text, int *occ, int num_occ, int text_size, int line_number, int patt_size, char *file_name,FILE *out_file){
    int counter = 0;
    int cursor = 0;
    blue();
    if(is_out_file) fprintf(out_file,"\n%d Occurrences in Line %d of %s\n",num_occ, line_number, file_name);
    else printf("\n%d Occurrences in Line %d of %s\n",num_occ, line_number, file_name);
    default_colour();
    for(int i = 0; i < text_size; i++){
        if(occ[cursor] == i){
            cursor++;
            counter = 1;
        }
        if(counter == 0 or counter == patt_size + 1){
            counter = 0;
            default_colour();
        }
        else{
            red();
            counter++;
        }
        if(is_out_file) fprintf(out_file,"%c",text[i]);
        else printf("%c",text[i]);
        
    }
}
alg_print_ret_sg send_to_func(alg_params_sg *params,char *text,int text_size,int patt_size,int line_number,int max_count){
    switch(params->func){
        case ALG_BOYLER_MOORE:
            break;
        case ALG_BRUTE_FORCE:
            return bruteforce(text,params->patt,patt_size,text_size,line_number,max_count,ignore_case);
            break;
        case ALG_SHIFT_OR:
            break;
        case ALG_SELLERS:
            break;
        case ALG_UKKONEN:
            break;
        default:
            return bruteforce(text,params->patt,patt_size,text_size,line_number,max_count,ignore_case);
            break;

    }

}
void *prepare_sg_func(void *args){
    alg_params_sg *params;
    params = (alg_params_sg*) args;
    int max_count = params->max_count;

    clock_t start, end;
    int size = 1;
    int num_occ = 0;
    
    char text[TEXT_MAX_SIZE];
    int patt_size = strlen(params->patt);
    int line_number = 0;
    FILE *txt_file = fopen(params->file_name,"r");
    FILE *out_file;
    if(is_out_file){
        out_file = fopen(params->out_file,"w");
    }
    start = clock();
    while(fgets(text,TEXT_MAX_SIZE,txt_file)){
        int text_size = strlen(text);
        alg_print_ret_sg temp = send_to_func(params,text,text_size,patt_size,line_number,max_count);
        num_occ += temp.num_occ;
        max_count -= temp.num_occ;
        if(max_count == 0) break;
        else{
            if(temp.num_occ > 0 and !only_count){
                pthread_mutex_lock(&global_mutex);
                print_occ(text,temp.occ,temp.num_occ,text_size,line_number,patt_size,params->file_name,out_file);
                pthread_mutex_unlock(&global_mutex);
            }
        }
        line_number += 1;
    }
    end = clock();
    double total_time = (double) (end - start) / CLOCKS_PER_SEC;
    if(only_count){
        pthread_mutex_lock(&global_mutex);
        if(is_out_file) fprintf(out_file,"\nIn %s, for %s: %d\n",params->file_name,params->patt,num_occ);
        else printf("\nIn %s, for %s: %d\n",params->file_name,params->patt,num_occ);
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