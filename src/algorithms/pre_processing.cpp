#include "stdio.h"
#include "pre_processing.hpp"
#include "string.h"

Node *fsm;
int **strict_nxt, **bad_char, **good_suffix;
int **aho_occ, *aho_qnt_occ;
int **aho_go_to, *aho_fail;
int size_goto = 0, occ_size = 0;
char *strrev(char *str, int str_len){
    int i = 0, j = str_len - 1;
    if(str_len <= 0) return str;
    char *temp = (char*) malloc(str_len * sizeof(char));
    strcpy(temp, str);
    while(i < j){
        swap(temp[i], temp[j]);
        i++, j--;
    }
    return temp;
}

bool isEqual(char a, char b, bool ignore_case){
    if(ignore_case){
        a = tolower(a);
        b = tolower(b);
    }
    return (a == b);
}

int* init_strict_next(char *pat, int patt_size, bool ignore_case, int *nxt){
    int m = patt_size;
    nxt = (int*) malloc((m + 1) * sizeof(int));
    for(int i = 0; i <= m; i++) nxt[i] = -1;

    if(m == 1 || (m > 1 && !isEqual(pat[0], pat[1], ignore_case))) nxt[1] = 0;

    int i = 1, j = 0;
    while(i + j < m){
        while(i + j < m and isEqual(pat[i + j], pat[j], ignore_case)){
            j++;
            if(i + j == m || !isEqual(pat[i + j], pat[j], ignore_case)) nxt[i + j] = j;
            else nxt[i + j] = nxt[j];
        }

        if(j == 0 && ((i + 1 == m) || (!isEqual(pat[0], pat[i + 1], ignore_case)))){
            nxt[i + 1] = 0;
        }

        i += (j - nxt[j]);
        j = max(0, nxt[j]);
        
    }

    return nxt;
}

int *init_bad_char(char *pat, int patt_size, bool ignore_case,int *bc){
    int l = alpha_len; 
    int m = patt_size;
    bc = (int*) malloc(l * sizeof(int));
    for(int i = 0; i < l; i++) bc[i] = -1;
    for(int i = 0; i < m; i++){
        bc[pat[i]] = i;
        if(ignore_case){ 
            bc[tolower(pat[i])] = i;
            bc[toupper(pat[i])] = i;
        }
    }
    return bc;
}

int** build_go_to(vector<char*> &pat_set, vector<int> &patt_size, int qnt_pat, int **occ, int *qnt_occ){
    int nxt = 1;
    int **go_to;
    
    occ = (int**) malloc(sizeof(int*));
    occ_size  = 1;
    go_to = (int**) malloc(sizeof(int*));
    size_goto = 1;

    go_to[0] = (int*) malloc(alpha_len * sizeof(int));

    qnt_occ = (int*) calloc(qnt_pat, sizeof(int));
    for(int i = 0; i < alpha_len; i++) go_to[0][i] = -1;

    for(int k = 0; k < qnt_pat; k++){
        
        char *pat = pat_set[k];
        int m = patt_size[k];

        int node = 0, j = 0;
        int ch = pat[j];

        while(j < m && go_to[node][ch] != -1){
            node = go_to[node][ch];
            j++;
            ch = pat[j];
        }
        while(j < m){

            ch = pat[j];
            go_to[node][ch] = nxt;

            size_goto++;
            go_to = (int**) realloc(go_to, size_goto * sizeof(int*));
            go_to[size_goto - 1] = (int*)malloc(alpha_len * sizeof(int));
            for(int i = 0; i < alpha_len; i++) go_to[size_goto - 1][i] = -1;

            occ_size++;
            occ = (int**) realloc(occ, occ_size * sizeof(int*));   
            node = nxt;
            j++, nxt++;
        }

        qnt_occ[node]++;
        occ[node] = (int*) realloc(occ[node], qnt_occ[node]*sizeof(int));
        occ[node][qnt_occ[node] - 1] = k;
    }
    for(int i = 0; i < alpha_len; i++){
        if(go_to[0][i] == -1) go_to[0][i] = 0;
    }
    return go_to;
}

int* build_failure(int **go_to){
    int *fail = (int*) malloc(alpha_len * sizeof(int));
    for(int i = 0; i < alpha_len; i++) fail[i] = -1;

    queue<int> Q;
    for(int i = 0; i < alpha_len; i++){
        int temp = go_to[0][i];
        if(temp > 0){
            Q.push(temp);
            fail[temp] = 0;
        }
    }
    while(!Q.empty()){
        int node = Q.front();
        Q.pop();
        for(int pos = 0; pos < alpha_len; pos++){
            int vertex = go_to[node][pos];
            
            if(vertex >= 0){
                Q.push(vertex);
                int f = fail[node];
                while(go_to[f][pos] < 0){
                    f = fail[f];
                }
                fail[vertex] = go_to[f][pos];
                int temp = aho_qnt_occ[vertex];
                aho_qnt_occ[vertex] += aho_qnt_occ[fail[vertex]];
                if(aho_qnt_occ[vertex] == 1) aho_occ[vertex] = (int*) malloc(aho_qnt_occ[vertex] * sizeof(int));
                else aho_occ[vertex] = (int*) realloc(aho_occ[vertex], aho_qnt_occ[vertex] * sizeof(int));
                for(int inc = temp; inc < aho_qnt_occ[vertex]; inc++){
                    aho_occ[vertex][inc] = aho_occ[fail[vertex]][inc - temp];
                }
            }
            
        }
    }

    return fail;
}

int *init_good_suffix(char *pat, int patt_size, bool ignore_case, int *gs,int *ni){
    
    int m = patt_size;
    char *temp = strrev(pat, m);
    int *rni = init_strict_next(temp, m, ignore_case, rni);
    gs = (int*) malloc((m + 1) * sizeof(int));
    for(int i = 0; i < m + 1; i++) gs[i] = m - ni[m];
    for(int i = 1; i < m; i++){
        int j = ((m - 1) - rni[i] + (m+1)) % (m+1);
        if(i - rni[i] < gs[j]) gs[j] = i - rni[i];
    }
    free(temp);
    free(rni);
    return gs; 
}

void preprocess(Args &pmt,vector<vector<int>> &alg_used){

    strict_nxt = (int**) malloc((pmt.num_patt) * sizeof(int*));
    bad_char = (int**) malloc((pmt.num_patt) * sizeof(int*));
    good_suffix = (int**) malloc((pmt.num_patt) * sizeof(int*));

    if(pmt.is_mult_patt and alg_used[0][0] == ALG_AHO_CORASICK){
        aho_go_to = build_go_to(pmt.patterns, pmt.patt_size, pmt.num_patt, aho_occ, aho_qnt_occ);
        aho_fail = build_failure(aho_go_to);
    }
    else if(pmt.is_mult_patt and alg_used[0][0] == ALG_WU_MANBER){

    }
    else{
        for(int i = 0; i < pmt.num_patt; i++){
            bool k = false, bm = false;
            for(auto &txt: alg_used){
                switch(txt[i]){
                    case ALG_KMP:
                        k = true;
                        break;
                    case ALG_BOYER_MOORE:
                        bm = true;
                        break;
                }
            }
            if(k or bm) strict_nxt[i] = init_strict_next(pmt.patterns[i], pmt.patt_size[i], ignore_case, strict_nxt[i]);
            if(bm){
                bad_char[i] = init_bad_char(pmt.patterns[i], pmt.patt_size[i], ignore_case, bad_char[i]);
                good_suffix[i] = init_good_suffix(pmt.patterns[i], pmt.patt_size[i], ignore_case, good_suffix[i], strict_nxt[i]);
            }
        }
    }
}