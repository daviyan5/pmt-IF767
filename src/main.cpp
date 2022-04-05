#include <iostream>
#include "pthread.h"
#include "./parse_utils/parse.hpp"
#include "./algorithms/algorithms.hpp"

using namespace std;

// A main serve de intermediário entre o parsing dos comandos e o manejamento dos algoritmos.
int main(int argc,char *argv[]){
    // Os comandos e os argumentos são gerados
    Args pmt_params = parse_commands(argc,argv);

    // Se houve alguma falha na etapa anterior, o programa fecha
    if(pmt_params.failed) exit(1);
    manage_algorithms(pmt_params);
    return 0;
}