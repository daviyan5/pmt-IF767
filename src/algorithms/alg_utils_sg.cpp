#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "pthread.h"
#include "time.h"

#include "brute_force.hpp"
#include "kmp.hpp"
#include "boyermoore.hpp"
#include "sellers.hpp"
#include "wumanber.hpp"

#include "algorithms.hpp"
#include "pre_processing.hpp"


#define TEXT_MAX_SIZE 1000000

// Função para printar as ocorrências no texto
void print_occ_sg(char *patt,char *text, int *occ, int num_occ, int text_size, int line_number, int patt_size, char *file_name,FILE *out_file){
    blue();
    if(is_out_file) fprintf(out_file,"\n%d Occurrences of %s in Line %d of %s\n",num_occ,patt, line_number, file_name);
    else printf("\n%d Occurrences of %s in Line %d of %s\n",num_occ,patt, line_number, file_name);
    default_colour();
    bool *paint = (bool*) calloc(text_size,sizeof(bool));
    int counter = 0;
    while(counter < num_occ and occ[counter] < 0){
        int dif = abs(occ[counter]);
        for(int i = dif; i < patt_size; i++) paint[i - dif] = true;
        counter++;
    }
    for(int i = 0; i < text_size; i++){
        
        if(counter < num_occ and occ[counter] == i){
            counter++;
            for(int j = 0; j < patt_size and i + j < text_size; j++) paint[i + j] = true;
        }
        if(paint[i]) red();
        else default_colour();
        if(is_out_file) fprintf(out_file,"%c",text[i]);
        else printf("%c",text[i]);
        
    }
    free(paint);
    default_colour();
    if(!is_out_file) printf("\n");
}

// Envia a linha para a função respectiva
alg_print_ret_sg send_to_func_sg(alg_params_sg *params,char *text,int text_size,int patt_size,int max_count){
    switch(params->func){
        case ALG_BOYER_MOORE:
            return boyermoore(text,params->patt,patt_size,text_size,max_count,ignore_case,bad_char[params->patt_idx],good_suffix[params->patt_idx]);
            break;
        case ALG_BRUTE_FORCE:
            return bruteforce(text,params->patt,patt_size,text_size,max_count,ignore_case);
            break;
        case ALG_KMP:

            return kmp(text,params->patt,patt_size,text_size,max_count,ignore_case,strict_nxt[params->patt_idx]);
            break;
        case ALG_SELLERS:
            return sellers(text,params->patt,patt_size,text_size,max_count,dist,ignore_case);
            break;
        case ALG_WU_MANBER:
            return wumanber(text,params->patt,patt_size,text_size,max_count,dist,ignore_case,C_wu[params->patt_idx]);
            break;
        default:
            return boyermoore(text,params->patt,patt_size,text_size,max_count,ignore_case,bad_char[params->patt_idx],good_suffix[params->patt_idx]);
            break;
    }
    return boyermoore(text,params->patt,patt_size,text_size,max_count,ignore_case,bad_char[params->patt_idx],good_suffix[params->patt_idx]);
}

// Função principal, envia a thread com a linha para a função de casamento
void *prepare_sg_func(void *args){
    clock_t start, end;             // Contagem de tempo de execução da thread
    start = clock();     
    alg_params_sg *params;
    params = (alg_params_sg*) args;
    int max_count = params->max_count;

   
    int num_occ = 0;
    
    char text[TEXT_MAX_SIZE];
    int patt_size = params->patt_size;
    int line_number = 0;
    FILE *txt_file = fopen(params->file_name,"r");
    if(!txt_file){
        printf("Erro ao abrir arquivo de texto!\n");
        exit(1);
    }
    FILE *out_file;
    if(is_out_file){
        out_file = fopen(params->out_file,"w");
        if(!out_file){
            printf("Erro ao abrir arquivo de outputs!\n");
            exit(1);
        }
    }
    

    // Leitura do texto
    while(fgets(text,TEXT_MAX_SIZE,txt_file)){
        int text_size = strlen(text);
        alg_print_ret_sg temp = send_to_func_sg(params,text,text_size,patt_size,max_count);
        num_occ += temp.num_occ;
        max_count -= temp.num_occ;
        if(max_count == 0) break;
        else{
            if(temp.num_occ > 0 and !only_count){
                pthread_mutex_lock(&global_mutex);
                print_occ_sg(params->patt,text,temp.occ,temp.num_occ,text_size,line_number,patt_size,params->file_name,out_file);
                pthread_mutex_unlock(&global_mutex);
            }
        }
        line_number += 1;
        free(temp.occ);
    }
    
    if(only_count){
        pthread_mutex_lock(&global_mutex);
        if(is_out_file) fprintf(out_file,"\nIn %s, for %s: %d\n",params->file_name,params->patt,num_occ);
        else printf("\nIn %s, for %s: %d\n",params->file_name,params->patt,num_occ);
        pthread_mutex_unlock(&global_mutex);
    }
    end = clock(); 
    double total_time = (double) (end - start) / CLOCKS_PER_SEC;
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