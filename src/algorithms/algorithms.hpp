#ifndef _HAVE_ALGO_H
#define _HAVE_ALGO_H
#include <iostream>
#include <pthread.h>
#include "../parse_utils/parse.hpp"
#include "pre_processing.hpp"

// Aqui fazemos global os paramêtros recebidos pela CLI
extern bool is_out_file;
extern bool only_count;
extern bool ignore_case;
extern bool show_stt;
extern int dist;

// Mutex para controle de output (stdin ou arquivo de output)
extern pthread_mutex_t global_mutex;

// Struct que é enviada para cada thread que lida com apenas um parâmetro por texto
struct alg_params_sg{
    char *patt;         // Padrão
    char *file_name;    // Nome do arquivo de texto que a thread lidará
    char *out_file;     // Nome do arquivo de output que a thread lidará

    int patt_size;      // Tamanho do padrão
    int patt_idx;       // Índice do padrão, no vetor de padrões do argumento. Utilizado para envio dos pre-processamentos respectivos àquele padrão
    int max_count;      // Máximo de aparições permitidas daquele padrão, naquele texto
    int func;           // Número do algoritmo utilizado pela thread
};

// Struct que é enviada para cada thread que lida com vários parâmetros por texto
struct alg_params_mul{
    char **patt;        // Array de padrões
    char *file_name;    // Nome do arquivo de texto que a thread lidará
    char *out_file;     // Nome do arquivo de output que a thread lidará
    int *patt_size;     // Tamanho de cada padrão
    
    int num_patt;       // Número de padrões
    int func;           // Número do algoritmo utilizado pela thread
};

// Retorno das funções de apenas um padrão
struct alg_print_ret_sg{
    
    int *occ;           // Índice  
    int num_occ;        // Número de ocorrências
};
struct alg_print_ret_mul{
    int **occ;          // Índice das ocorrências, por padrão
    int *num_occ;       // Número de ocorrências, por padrão
};
void blue();            
void red();             
void default_colour();  
void manage_algorithms(Args &pmt);  // Escolhe e cria as threads para cada padrão/texto



#endif