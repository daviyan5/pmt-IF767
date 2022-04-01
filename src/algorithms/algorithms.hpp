#ifndef _HAVE_ALGO_H
#define _HAVE_ALGO_H
#include <iostream>
#include <pthread.h>
#include "../parse_utils/parse.hpp"

extern bool is_out_file;
extern bool only_count;
extern bool ignore_case;
extern bool show_stt;
extern int dist;

extern pthread_mutex_t global_mutex;

struct alg_params_sg{           // Envio das threads
    char *patt;
    char *file_name;
    char *out_file;

    int max_count;
    int func;
};
struct alg_params_mul{           // Envio das threads
    char **patt;
    char *file_name;
    char *out_file;

    int max_count;
    int func;
};
struct alg_print_ret_sg{
    int *occ;
    int num_occ;
};
struct alg_print_ret_mul{
    int **occ;
    int *num_occ;
};
void manage_algorithms(Args &pmt);  // Escolhe e cria as threads para cada padrão/texto



#endif