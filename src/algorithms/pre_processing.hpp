#ifndef H_PRE_PROCESS
#define H_PRE_PROCESS
#include "algorithms.hpp"


extern int **strict_nxt, **bad_char, **good_suffix;
extern int **aho_occ, *aho_qnt_occ;
extern int **aho_go_to, *aho_fail;
extern int size_goto, occ_size;
const int alpha_len = 256;

struct Node{
    int nxt_aho[alpha_len];
    bool isLeaf = false;

    int parent = -1;
    char parent_char;
    int link = -1;
    int go[alpha_len];

    Node(int p = -1, char c = '$'): parent(p), parent_char(c){
        memset(nxt_aho, -1, sizeof(nxt_aho));
        memset(go, -1, sizeof(go));
    }
};
extern Node *fsm;
bool isEqual(char a, char b, bool ignore_case);
void preprocess(Args &pmt,vector<vector<int>> &alg_used);

#endif 