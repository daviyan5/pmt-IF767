#include <iostream>
#include <getopt.h>
#include <fstream>
#include "parse.hpp"

using namespace std;

void print_helper(){

}
Args parse_commands(int argc,char *argv[]){
    Args pmt_args;
    int opt;
    bool alg_aprox = false;
    int dist_aprox = -1;
    char *alg;
    while((opt = getopt(argc, argv, "he:a:")) != -1){ 
        switch(opt){ 
            case 'a':
                printf("Algoritmo %s\n",optarg);
                alg = optarg;
                break;
            case 'e':
                alg_aprox = true;
                dist_aprox = atoi(optarg);
                printf("Distancia %s\n",optarg);
                break;
            case 'h':
                print_helper();
                pmt_args.type = TYPE_INFORMATIVE;
                break;
            case ':': 
                printf("A opção necessita de um valor!\nDigite -h ou --help para mais detalhes\n"); 
                break; 
            case '?': 
                printf("Opção desconhecida: %c\nDigite -h ou --help para mais detalhes\n", optopt);
                break; 
        }
    }
    return pmt_args;
}   
