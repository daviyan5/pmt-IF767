#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "pthread.h"
#include <chrono>

#include "aho_corasick.hpp"

#include "algorithms.hpp"
#include "pre_processing.hpp"


#define TEXT_MAX_SIZE 1000000
// Função para printar as ocorrências no texto
void print_occ_mul(char *text, alg_print_ret_mul &temp, int total_occ, int text_size,int num_patt, int line_number, int *patt_size, 
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
            if(counter[j] < temp.num_occ[j] and temp.occ[j][counter[j]] == i){
                int pos = temp.occ[j][counter[j]];
                counter[j]++;
                for(int k = 0; k < patt_size[j] and k + pos < text_size; k++) paint[k+pos] = true;
            }
        }
        if(paint[i]) red();
        else default_colour();
        if(is_out_file) fprintf(out_file,"%c",text[i]);
        else printf("%c",text[i]);
        
    }
    free(paint);
    free(counter);
    default_colour();
    if(!is_out_file) printf("\n");
}
// Envia a linha para a função respectiva
alg_print_ret_mul send_to_func_mul(alg_params_mul *params,char *text,int text_size,int *patt_size){
    switch(params->func){
        case ALG_AHO_CORASICK:
            return aho_corasick(text,patt_size,params->num_patt,text_size,ignore_case,aho_go_to,aho_fail,aho_qnt_occ,aho_occ);
            break;
    }
    return aho_corasick(text,patt_size,params->num_patt,text_size,ignore_case,aho_go_to,aho_fail,aho_qnt_occ,aho_occ);
}
// Limpa a memória utilizada pela struct temp
void free_temp(alg_print_ret_mul &temp,int num_patt){
    for(int i = 0; i < num_patt; i++){
        free(temp.occ[i]);
    }
    free(temp.occ);
    free(temp.num_occ);
}

// Função de thread que envia para o casamento de múltiplos padrões
void *prepare_mul_func(void *args){
    auto t_start = std::chrono::high_resolution_clock::now();    // Tempo de execução
    alg_params_mul *params;
    params = (alg_params_mul*) args;

    
    int *num_occ = (int*) calloc(params->num_patt,sizeof(int));
    
    char text[TEXT_MAX_SIZE];
    int *patt_size = params->patt_size;
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
    int total_occ = 0;
    

    // Leitura do texto
    while(fgets(text,TEXT_MAX_SIZE,txt_file)){

        int text_size = strlen(text);
        alg_print_ret_mul temp = send_to_func_mul(params,text,text_size,patt_size);
        
        bool some_occ = temp.total_occ > 0;
        int local_occ = temp.total_occ;
        total_occ += local_occ;
        if(some_occ  and !only_count){
            pthread_mutex_lock(&global_mutex);
            print_occ_mul(text,temp,local_occ,text_size,params->num_patt,line_number,patt_size,params->file_name,out_file);
            pthread_mutex_unlock(&global_mutex);
        }
        line_number += 1;
        free_temp(temp,params->num_patt);
    }
    
    
    
    if(only_count){
        pthread_mutex_lock(&global_mutex);
        if(is_out_file) fprintf(out_file,"\nIn %s: %d total occurrences.\n",params->file_name,total_occ);
        else printf("\nIn %s: %d total occurrences.\n",params->file_name,total_occ);
        pthread_mutex_unlock(&global_mutex);
    }
    auto t_end = std::chrono::high_resolution_clock::now();
    double total_time = std::chrono::duration<double, std::milli>(t_end-t_start).count() / 1000;
    if(show_stt){
        pthread_mutex_lock(&global_mutex);
        if(is_out_file) fprintf(out_file,"Spent time: %lf s\n",total_time);
        else printf("Spent time: %lf s\n",total_time);
        pthread_mutex_unlock(&global_mutex);
    }
    free(num_occ);
    fclose(txt_file);
    if(is_out_file) fclose(out_file);
    pthread_exit(NULL);
}