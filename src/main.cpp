#include <iostream>
#include "./parse_utils/parse.hpp"

using namespace std;

int main(int argc,char *argv[]){
    Args pmt_params = parse_commands(argc,argv);
    return 0;
}