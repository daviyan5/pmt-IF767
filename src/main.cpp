#include <iostream>
#include "pthread.h"
#include "./parse_utils/parse.hpp"
#include "./algorithms/algorithms.hpp"

using namespace std;

int main(int argc,char *argv[]){
    Args pmt_params = parse_commands(argc,argv);
    if(pmt_params.failed) exit(1);
    manage_algorithms(pmt_params);
    return 0;
}