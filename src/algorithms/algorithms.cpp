#include <iostream>
#include <bits/stdc++.h>
#include "../parse_utils/parse.hpp"
#include "algorithms.hpp"
#include "brute_force.hpp"

#include "pthread.h"
using namespace std;



vector<vector<alg_params_sg>> threads_params_sg;
vector<alg_params_mul> threads_params_mul;
vector<vector<alg_ret_sg>> threads_ret_sg;
vector<alg_ret_mul> threads_ret_mul;

vector<void *(*)(void*)> algs = {NULL, bruteforce, NULL, NULL, NULL, NULL, NULL};
int decide_alg(Args &pmt,int text,int patt){
    return ALG_BRUTE_FORCE;
}

void init_params_sg(Args &pmt){
    threads_params_sg.resize(pmt.num_txt);
    threads_ret_sg.resize(pmt.num_txt);
    for(int i = 0; i < pmt.num_txt; i++){
        threads_params_sg[i].resize(pmt.num_patt);
        threads_ret_sg[i].resize(pmt.num_patt);
        for(int j = 0; j < pmt.num_patt; j++){
            threads_params_sg[i][j].dist = pmt.dist;
            threads_params_sg[i][j].patt = (char *) malloc((strlen(pmt.patterns[j]) + 1) * sizeof(char));
            strcpy(threads_params_sg[i][j].patt,pmt.patterns[j]);
        }
    }
}
void init_params_mul(Args &pmt){        // TODO
    threads_params_mul.resize(pmt.num_txt);
    threads_ret_mul.resize(pmt.num_txt);
    for(int i = 0; i < pmt.num_txt; i++){
       
    }
}

void debug(vector<vector<int>> &alg_used, Args &pmt){
    for(int text = 0; text < pmt.num_txt; text++){
        for(int patt = 0; patt < pmt.num_patt; patt++){
           printf("Algoritmo usado no texto %s padrão %s = %s\n",pmt.text_files[text],pmt.patterns[patt],get_alg_name(alg_used[text][patt]));
           int dist = threads_params_sg[text][patt].dist;
           printf("Parametros da thread, padrao = %s, dist = %d\n",threads_params_sg[text][patt].patt,dist);
        }
    }
    printf("\n#########################################################################\n\n");

}
void free_all(Args &pmt){
    for(int i = 0; i < pmt.num_txt; i++){
        for(int j = 0; j < pmt.num_patt; j++){
            free(threads_params_sg[i][j].patt);
            free(threads_params_sg[i][j].txt);
            free(threads_ret_sg[i][j].occ);
        }
    }
}

void manage_algorithms(Args &pmt){
    vector<vector<int>> alg_used(pmt.num_txt,vector<int>(pmt.num_patt,pmt.alg));

    if(pmt.alg == -1){  
        for(int text = 0; text < pmt.num_txt; text++){
            for(int patt = 0; patt < pmt.num_patt; patt++){
                alg_used[text][patt] = decide_alg(pmt,text,patt);
            }
        }
    }
    
    pthread_t pmt_threads[pmt.num_txt][pmt.num_patt];
    bool is_mult_alg = pmt.is_mult_patt and pmt.alg == -1 and (alg_used[0][0] == ALG_AHO_CORASICK or alg_used[0][0] == ALG_WU_MANBER);
    if(is_mult_alg){
        init_params_mul(pmt);
    }
    else{
        init_params_sg(pmt);
    }
    
    int text_number = 0;
    for(auto &text_file:pmt.text_files){
        ifstream temp_input(text_file);
        stringstream buffer;
        buffer << temp_input.rdbuf();
        
        string temp = buffer.str();
        temp.pop_back();
        char *text = (char*) malloc((temp.size()+1) * sizeof(char));
        strcpy(text,temp.c_str());
        temp_input.close();
        if(is_mult_alg){
            // TODO
        }
        else{
            int patt_number = 0;
            int a_use = alg_used[text_number][patt_number];
            for(auto u:pmt.patterns){
                threads_params_sg[text_number][patt_number].txt = (char *) malloc((temp.size() + 1) * sizeof(char));
                strcpy(threads_params_sg[text_number][patt_number].txt,text);
                pthread_create(&pmt_threads[text_number][patt_number],NULL,
                               algs[a_use],&threads_params_sg[text_number][patt_number]);
                patt_number++;   
            }

        }
        free(text);
        text_number += 1;
    }
    for(int i = 0; i < pmt.num_txt;i++){
        if(is_mult_alg){

        }
        else{
            for(int j = 0; j < pmt.num_patt; j++){
                alg_ret_sg *temp;
                int ret = pthread_join(pmt_threads[i][j],(void **)&temp);
                if(ret) printf("Erro na thread!\n");
                threads_ret_sg[i][j] = *temp;
                free(temp);
            }
        }  
    }
    if(pmt.show_info) debug(alg_used,pmt);
    for(int i = 0; i < pmt.num_txt;i++){
        if(is_mult_alg){

        }
        else{
            for(int j = 0; j < pmt.num_patt; j++){
                printf("No texto %s, com o padrão %s, demorou %lf segundos e houveram %d ocorrências\n",
                        pmt.text_files[i],pmt.patterns[j],threads_ret_sg[i][j].time,threads_ret_sg[i][j].num);
            }
        }  
    }
    /*
        TODO:
            1 - Tratar algoritmos de múltiplos padrões
            2 - Finalizar Outputs
            3 - Lidar com as flags de opções
            4 - Dar free nas coisas
            5 - Corrigir bugs (lol)
    */
    free_all(pmt);

}