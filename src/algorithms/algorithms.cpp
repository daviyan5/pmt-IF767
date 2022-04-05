#include <iostream>
#include <bits/stdc++.h>
#include "../parse_utils/parse.hpp"
#include "algorithms.hpp"
#include "alg_utils_sg.hpp"
#include "alg_utils_mul.hpp"
#include "pre_processing.hpp"

#include "pthread.h"
using namespace std;

//Comentados no Header

bool is_out_file;
bool only_count;
bool ignore_case;
bool show_stt;
int dist;


pthread_mutex_t global_mutex = PTHREAD_MUTEX_INITIALIZER;

vector<vector<alg_params_sg>> threads_params_sg;    // Vetor com os parâmetros de cada thread que recebe somente um padrão.
vector<alg_params_mul> threads_params_mul;          // Vetor com os parâmetros de cada thread que recebe somente mais de padrão.

// Deixa a cor do texto no terminal azul 
void blue(){
    printf("\x1b[34m");
}

// Deixa a cor do texto no terminal vermelho
void red(){
    printf("\x1b[31m");
}

// Reseta a cor do terminal
void default_colour(){
    printf("\x1b[0m");
}

// Decide o algoritmo utilizado por cada thread
int decide_alg(Args &pmt, int text_num, int patt_idx){
    if(pmt.dist == 0 and pmt.is_mult_patt and !pmt.force_sg){
        return ALG_AHO_CORASICK;
    }
    else{
        if(dist > 0){
            if(pmt.dist >= 64) return ALG_SELLERS;
            else return ALG_WU_MANBER;
        }
        else{
            if(pmt.patt_size[patt_idx] < 3) return ALG_BRUTE_FORCE;
            else if(pmt.patt_size[patt_idx] >= 4 and pmt.patt_size[patt_idx] < 7 )return ALG_KMP;
            else return ALG_BOYER_MOORE;
        }
    }
}

// Inicializa os parâmetros de cada thread, de acordo com o padrão e o texto que será lido por ela
void init_threads_sg(Args &pmt,vector<vector<int>> &alg_used,pthread_t **pmt_threads){
    threads_params_sg.resize(pmt.num_txt);  

    for(int i = 0; i < pmt.num_txt; i++){
        threads_params_sg[i].resize(pmt.num_patt);

        for(int j = 0; j < pmt.num_patt; j++){

            threads_params_sg[i][j].patt = (char *) malloc((pmt.patt_size[j] + 2) * sizeof(char));
            strcpy(threads_params_sg[i][j].patt,pmt.patterns[j]);

            threads_params_sg[i][j].file_name = (char *) malloc((strlen(pmt.text_files[i]) + 2) * sizeof(char));
            strcpy(threads_params_sg[i][j].file_name, pmt.text_files[i]);

            if(is_out_file){
                char *f_temp = (char*) malloc((500 * sizeof(char)));
                threads_params_sg[i][j].out_file = (char*) malloc((500 * sizeof(char)));
                sprintf(f_temp, "file%d_patt%d", i, j); 
                strcpy(threads_params_sg[i][j].out_file,pmt.out_file);
                strcat(threads_params_sg[i][j].out_file,f_temp);
            }
            threads_params_sg[i][j].patt_idx = j;
            threads_params_sg[i][j].patt_size = pmt.patt_size[j];
            threads_params_sg[i][j].max_count = pmt.max_count;
            threads_params_sg[i][j].func = alg_used[i][j];
    
            pthread_create(&pmt_threads[i][j],NULL,prepare_sg_func,&threads_params_sg[i][j]);
        }
    }
}
// Inicializa os parâmetros de cada thread, de acordo com o padrão e o texto que será lido por ela
void init_threads_mul(Args &pmt,vector<vector<int>> &alg_used,pthread_t **pmt_threads){ 
    threads_params_mul.resize(pmt.num_txt);

    for(int i = 0; i < pmt.num_txt; i++){
        threads_params_mul[i].num_patt = pmt.num_patt;
        threads_params_mul[i].patt = (char**) malloc((pmt.num_patt+1) * sizeof(char*));
        threads_params_mul[i].patt_size = (int*) malloc(pmt.num_patt * sizeof(int));
        for(int j = 0; j < pmt.num_patt; j++){
            threads_params_mul[i].patt[j] = (char*) malloc((pmt.patt_size[j]+1) * sizeof(char));
            strcpy(threads_params_mul[i].patt[j],pmt.patterns[j]);
            threads_params_mul[i].patt_size[j] = pmt.patt_size[j];
        }
        threads_params_mul[i].file_name = (char *) malloc((strlen(pmt.text_files[i]) + 1) * sizeof(char));
        strcpy(threads_params_mul[i].file_name, pmt.text_files[i]);
        if(is_out_file){
            char *f_temp = (char*) malloc((500 * sizeof(char)));
            threads_params_mul[i].out_file = (char*) malloc((500 * sizeof(char)));
            sprintf(f_temp, "file%d_multpatt", i); 
            strcpy(threads_params_mul[i].out_file,pmt.out_file);
            strcat(threads_params_mul[i].out_file,f_temp);
            free(f_temp);
        }

        threads_params_mul[i].func = alg_used[i][0];

        pthread_create(&pmt_threads[i][0],NULL,prepare_mul_func, &threads_params_mul[i]);
    }
}
// Função de debug, somente executada pela flag -n ou --show
void debug(vector<vector<int>> &alg_used, Args &pmt){
    for(int text = 0; text < pmt.num_txt; text++){
        for(int patt = 0; patt < pmt.num_patt; patt++){
           printf("Algoritmo usado no texto %s padrão %s = %s\n",pmt.text_files[text],pmt.patterns[patt],get_alg_name(alg_used[text][patt]));
           
        }
    }
    printf("\n#########################################################################\n\n");

}
// Limpa todos os ponteiros utilizados pelas threads de apenas um padrão
void free_all_sg(Args &pmt,vector<vector<int>> &alg_used){
    free(pmt.out_file);
    for(int i = 0; i < pmt.num_txt; i++){
        for(int j = 0; j < pmt.num_patt; j++){
            free(threads_params_sg[i][j].patt);
            free(threads_params_sg[i][j].file_name);
            if(pmt.is_out_txt) free(threads_params_sg[i][j].out_file);            
        }
    }
    for(int j = 0; j < pmt.num_patt; j++){
        if(alg_used[0][j] == ALG_KMP or alg_used[0][j] == ALG_BOYER_MOORE) free(strict_nxt[j]);
        if(alg_used[0][j] == ALG_BOYER_MOORE){
            free(good_suffix[j]);
            free(bad_char[j]);
        }
        if(alg_used[0][j] == ALG_WU_MANBER){
            free(C_wu[j]);
        }
    }
    free(C_wu);
    free(strict_nxt);
    free(good_suffix);
    free(bad_char);
}

// Limpa todos os ponteiros utilizados pelas threads de múltiplos padrões
void free_all_mul(Args &pmt){
    free(pmt.out_file);
    for(int i = 0; i < pmt.num_txt; i++){
        free(threads_params_mul[i].file_name);
        if(pmt.is_out_txt) free(threads_params_mul[i].out_file);
        free(threads_params_mul[i].patt_size);
        for(int j = 0; j < pmt.num_patt; j++){
            free(threads_params_mul[i].patt[j]);
        }
        free(threads_params_mul[i].patt);
    }
    free(strict_nxt);
    free(good_suffix);
    free(bad_char);
    free(C_wu);
    for(int i = 0; i < occ_size; i++){
        free(aho_occ[i]);
    }
    free(aho_occ);
    for(int i = 0; i< size_goto; i++){
        free(aho_go_to[i]);
    }
    free(aho_go_to);
    free(aho_fail);
    free(aho_qnt_occ);
}
// Função principal do gerenciamento das threads
void manage_algorithms(Args &pmt){
    vector<vector<int>> alg_used(pmt.num_txt+1,vector<int>(pmt.num_patt+1,pmt.alg));
    is_out_file = pmt.is_out_txt;
    only_count = pmt.only_count;
    ignore_case = pmt.ignore_case;
    show_stt = pmt.show_stt;
    dist = pmt.dist;
    
    if(pmt.alg == -1){  
        for(int text = 0; text < pmt.num_txt; text++){
            for(int patt = 0; patt < pmt.num_patt; patt++){
                alg_used[text][patt] = decide_alg(pmt,text,patt);
            }
        }
    }
    // Faz o preprocessamento de todos os algoritmos utilizados
    preprocess(pmt,alg_used);
    
    pthread_t **pmt_threads;
    pmt_threads = (pthread_t**) malloc(pmt.num_txt * sizeof (pthread_t*));
    for(int i = 0; i < pmt.num_txt; i++) pmt_threads[i] = (pthread_t*) malloc(pmt.num_patt * sizeof (pthread_t));

    // Booleano para saber se o algoritmo utilizado é de múltiplos padrões 
    bool is_mult_alg = alg_used[0][0] == ALG_AHO_CORASICK;

    if(pmt.show_info) debug(alg_used,pmt);

    // Inicializa cada thread de acordo com suas funções
    if(is_mult_alg){
        init_threads_mul(pmt,alg_used,pmt_threads);
    }
    else{
        init_threads_sg(pmt,alg_used,pmt_threads);
    }

    // Finaliza a execução de todas as threads
    for(int i = 0; i < pmt.num_txt;i++){
        if(is_mult_alg){
            int ret = pthread_join(pmt_threads[i][0],NULL);
            if(ret) printf("Erro na thread!\n");
        }
        else{
            for(int j = 0; j < pmt.num_patt; j++){
                int ret = pthread_join(pmt_threads[i][j],NULL);
                if(ret) printf("Erro na thread!\n");
            }
            
        }
        free(pmt_threads[i]);  
    }
    free(pmt_threads);
    pthread_mutex_destroy(&global_mutex);
    if(is_mult_alg) free_all_mul(pmt);
    else free_all_sg(pmt,alg_used);
    for(int i = 0; i < pmt.num_patt; i++){
        if(pmt.patterns[i]) free(pmt.patterns[i]);
    }
}