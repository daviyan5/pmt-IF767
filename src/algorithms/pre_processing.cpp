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

void build_go_to(vector<char*> &pat_set, vector<int> &patt_size, int qnt_pat){
    int nxt = 1;
    vector<vector<int>> temp_occ,temp_go_to;

    temp_go_to.push_back(vector<int>(alpha_len,-1));
    temp_occ.push_back(vector<int>());
    for(int k = 0; k < qnt_pat; k++){
        
        char *pat = pat_set[k];
        int m = patt_size[k];

        int node = 0, j = 0;
        int ch = pat[j];

        while(j < m && temp_go_to[node][ch] != -1){
            node = temp_go_to[node][ch];
            j++;
            ch = pat[j];
        }
        while(j < m){
            ch = pat[j];
            temp_go_to[node][ch] = nxt;
            temp_go_to.push_back(vector<int>(alpha_len,-1));
            temp_occ.push_back(vector<int>());
            node = nxt;
            j++;
            nxt += 1;
        }
        temp_occ[node].push_back(k);
    }
    for(int i = 0; i < alpha_len; i++){
        if(temp_go_to[0][i] == -1) temp_go_to[0][i] = 0;
    }
    size_goto = temp_go_to.size();
    aho_go_to = (int **) malloc(size_goto * sizeof(int*));
    for(int i = 0; i < size_goto; i++){
        aho_go_to[i] = (int *) malloc(alpha_len * sizeof(int));
        for(int j = 0; j < alpha_len; j++){
            aho_go_to[i][j] = temp_go_to[i][j];
            
        }
    }
    occ_size = temp_occ.size();
    aho_qnt_occ = (int *) malloc(occ_size * sizeof(int));
    aho_occ = (int **) malloc(occ_size * sizeof(int*));
    for(int i = 0; i < occ_size; i++){
        aho_qnt_occ[i] = temp_occ[i].size();
        aho_occ[i] = (int *) malloc(aho_qnt_occ[i] * sizeof(int));
        for(int j = 0; j < aho_qnt_occ[i]; j++){
            aho_occ[i][j] = temp_occ[i][j];
            if(temp_occ[i][j] != -1) printf("occ[%d][%d] = %d\n",i,j,temp_occ[i][j]);
        }
    }
}

void build_failure(int **go_to){
    aho_fail = (int*) malloc(alpha_len * sizeof(int));
    for(int i = 0; i < alpha_len; i++) aho_fail[i] = -1;

    queue<int> Q;
    for(int i = 0; i < alpha_len; i++){
        int temp = go_to[0][i];
        if(temp > 0){
            Q.push(temp);
            aho_fail[temp] = 0;
        }
    }
    while(!Q.empty()){
        int node = Q.front();
        Q.pop();
        for(int pos = 0; pos < alpha_len; pos++){
            int vertex = go_to[node][pos];
            
            if(vertex >= 0){
                Q.push(vertex);
                int f = aho_fail[node];

                while(go_to[f][pos] < 0){
                    f = aho_fail[f];
                }

                aho_fail[vertex] = go_to[f][pos];
                int temp = aho_qnt_occ[vertex];
                aho_qnt_occ[vertex] += aho_qnt_occ[aho_fail[vertex]];

                aho_occ[vertex] = (int*) realloc(aho_occ[vertex], aho_qnt_occ[vertex] * sizeof(int));

                for(int inc = temp; inc < aho_qnt_occ[vertex]; inc++){
                    aho_occ[vertex][inc] = aho_occ[aho_fail[vertex]][inc - temp];
                }
            }
            
        }
    }
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
        build_go_to(pmt.patterns, pmt.patt_size, pmt.num_patt);
        build_failure(aho_go_to);
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