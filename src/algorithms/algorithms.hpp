#ifndef _HAVE_ALGO_H
#define _HAVE_ALGO_H
#include <iostream>
#include "../parse_utils/parse.hpp"

struct alg_params_sg{           // Envio das threads
    char *patt;
    char *txt;
    int dist;
};
struct alg_ret_sg{              // Retorno das threads
    int *occ;
    int num;
    double time;
};
struct alg_params_mul{           // Envio das threads
    char **patt;
    char *txt;
    int dist;
};
struct alg_ret_mul{              // Retorno das threads
    int **occ;
    int *num;
    double time;
};
void manage_algorithms(Args &pmt);  // Escolhe e cria as threads para cada padrão/texto



#endif