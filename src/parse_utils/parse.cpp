#include <iostream>
#include <getopt.h>
#include <fstream>
#include "string.h"
#include "parse.hpp"

using namespace std;
const int PATT_MAX_SIZE = 100000;
int algs_number = 7;
char alg_names[][30] = {"boyermoore","bruteforce","kmp","ahocorasick","sellers","wumanber","ukkonen"};

char *get_alg_name(int pos){ return alg_names[pos];}
struct option long_options[] = {
    {"help", no_argument, NULL, 'h'},
	{"algorithm", required_argument, NULL, 'a'},
	{"edit", required_argument, NULL, 'e'},
    {"count", no_argument, NULL, 'c'},
    {"statistics",no_argument, NULL, 's'},
    {"pattern", required_argument, NULL, 'p'},
    {"max", required_argument, NULL, 'm'},  
    {"ignore", no_argument, NULL, 'i'},
    {"show", no_argument, NULL, 'n'},
    {"output", required_argument, NULL,'o'},

	{ 0, 0, 0, 0 }
};

void init_args(Args &pmt){
    pmt.only_help = 0;
    pmt.failed = false;
    pmt.only_count = false;   
    pmt.show_stt = false;      
    pmt.is_mult_patt = false;  
    pmt.is_out_txt = false;      
    pmt.ignore_case = false;       
    pmt.show_info = false;     
    pmt.is_patt_file = false;
    pmt.out_file = (char*) malloc((500 * sizeof(char)));
    strcpy(pmt.out_file,"./outputs/output_");
    pmt.dist = 0;
    pmt.num_patt = 0;
    pmt.num_txt = 0;
    pmt.num_threads = -1;
    pmt.alg = -1;
    pmt.max_count = -1;  
}

void print_option(const char command[],const char description[]){
    printf("   %-28s %s\n",command,description);
}

void print_helper(){
    printf("PMT: Casamento de Caracteres\n");
    printf("usage: ./pmt [options] pattern textfile [textfile...]\n\n");
    printf("options:\n");

    print_option("-h, --help:","Ajuda sobre o comando PMT\n");

    print_option("-e, --edit DIST:","Busca aproximada sobre distância DIST (se nenhum valor for enviado, 0 será utilizado)\n");

    print_option("-a, --algorithms ALG:","Especifica o algoritmo que deve ser utilizado para o casamento:");
    string temp = "";
    for(int i = 0; i < algs_number - 1; i++){
        temp += alg_names[i];
        temp += ',';
    }
    temp += alg_names[algs_number - 1];
    char f_temp[5000];
    sprintf(f_temp,"ALG pode ser: {%s}\n",temp.c_str());
    print_option("",f_temp);

    print_option("-c, --count:","Apenas conta o número de aparições do padrão no texto.\n");

    print_option("-s, --statistics:","Mostra estatísticas de tempo para cada texto ou padrão.\n");

    print_option("-p, --pattern PATTERNFILE:","Define um arquivo de padrões. Obs: Cada linha representa um padrão.\n");

    print_option("-m, --max MAX_COUNT:","Define o número máximo de aparições do padrão que o algoritmo deve procurar.\n");

    print_option("-i, --ignore:","Ignora o casing no casamento entre padrões.\n");

    print_option("-n, --show:","Mostra informações da instância e da execução.\n");

    print_option("-o, --output:","Salva o resultado em um arquivo de SAIDA. Esse arquivo pode ser encontrado em ./bin/outputs\n");


}

void print_info(Args &pmt){
    printf("\n##### INFORMAÇÕES #######################################################\n\n");
    if(!pmt.is_mult_patt){
        printf("Padrão procurado: %s\n",pmt.patterns[0]);
    }
    else{
        printf("Arquivo de padrões: %s\n",pmt.patt_file.c_str());
        int i = 0;
        for(auto &u:pmt.patterns){
            printf("Padrão %03d: %s\n",i,u);
            i++;
        }
    }
    int temp_idx = 0;
    for(auto u:pmt.text_files){
        printf("Texto %03d: %s\n",temp_idx,u);
        temp_idx++;
    }
    printf("Num de Arquivos de Busca: %d\n",pmt.num_txt);
    printf("Num de Threads: %d\n",pmt.num_threads);
    printf("Distância do Padrão: %d\n",pmt.dist);
    printf("Algoritmo Usado: %s\n",pmt.alg != -1? alg_names[pmt.alg]: "ndef");
    printf("Número máximo de busca: %d\n",pmt.max_count);
    if(pmt.is_out_txt) printf("Arquivo de saída: %s\n",pmt.out_file);
    
}

int parse_alg(char *alg_name){
    for(int i = 0; i < algs_number; i++){
        if(strcmp(alg_name, alg_names[i]) == 0) return i;
    }
    return -1;
}

void read_pattern_file(Args &pmt){
    FILE *file;
    file = fopen(pmt.patt_file.c_str(), "r");
    char temp[PATT_MAX_SIZE];
    while(fgets(temp, PATT_MAX_SIZE,file)){ 
        int len = strlen(temp);
        if(temp[len - 1] == '\n'){
            temp[len - 1] = '\0';
            len -= 1;
        }
        char *c = (char*) malloc((len + 1) * sizeof(char));
        strcpy(c, temp);
        pmt.patterns.push_back(c);
        pmt.num_patt += 1;
    }
    if(pmt.patterns.size() > 0) pmt.is_mult_patt = true;
    fclose(file);
}

Args parse_commands(int argc,char *argv[]){
    Args pmt_args;
    init_args(pmt_args);
    int opt;
    while((opt = getopt_long(argc, argv, "he:a:csp:im:now",long_options,NULL)) != -1){
        if(opt == 'h' and pmt_args.only_help == 0) pmt_args.only_help = 1;
        else pmt_args.only_help = 2;
        switch(opt){ 
            case 'a':
                pmt_args.alg = parse_alg(optarg);
                if(pmt_args.alg == -1) printf("Opção Inválida!\n");
                break;
            case 'e':
                pmt_args.dist = atoi(optarg);
                break;
            case 'h':
                print_helper();
                break;
            case 'c':
                pmt_args.only_count = true;
                break;
            case 's':
                pmt_args.show_stt = true;
                break;
            case 'p':
                pmt_args.patt_file = optarg;
                pmt_args.is_patt_file = true;
                break;
            case 'i':
                pmt_args.ignore_case = true;
                break;
            case 'm':
                pmt_args.max_count = atoi(optarg);
                break;
            case 'n':
                pmt_args.show_info = true;
                break;
            case 'o':
                pmt_args.is_out_txt = true;
                break;
            
        }
    }
    int idx = optind;
    if(!pmt_args.is_patt_file){ // Se o padrão não tiver sido pego em texto
        if(argc <= idx){        // Se não houver mais nenhum argumento
            if(pmt_args.only_help != 1) printf("Argumentos faltantes!\nAdicione os comandos -h ou --help para receber ajuda.\n");
            pmt_args.failed = true;
            return pmt_args;
        }
        else{
            pmt_args.patterns.push_back(argv[idx]);
            pmt_args.num_patt = 1;
            idx++;              // Proximo argumento
        }
    }
    else read_pattern_file(pmt_args);

    if(argc <= idx){
        printf("Argumentos faltantes!\nAdicione os comandos -h ou --help para receber ajuda.\n");
        pmt_args.failed = true;
        return pmt_args;
    }
    else{
        for(int i = idx; i < argc; i++){
            pmt_args.num_txt += 1;
            pmt_args.text_files.push_back(argv[i]);
        }
    }
    for(auto &patt:pmt_args.patterns){
        pmt_args.patt_size.push_back(strlen(patt));
    }
    pmt_args.num_threads = pmt_args.num_txt * pmt_args.num_patt;
    if(pmt_args.show_info) print_info(pmt_args);
    return pmt_args;
}
