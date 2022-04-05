#include "stdio.h"
#include "pre_processing.hpp"
#include "string.h"

int **strict_nxt, **bad_char, **good_suffix; // Next de borda estrita, mau caractere e bom sufixo
int **aho_occ, *aho_qnt_occ;                 // Ocorrências (isLeaf) e número de ocorrências por nó da Trie
int **aho_go_to, *aho_fail;                  // Filhos dos nós e array de fail
int size_goto = 0, occ_size = 0;             // Tamanho da Trie e tamanho do vetor de ocorrências
long long **C_wu;

char *strrev(char *str, int str_len){       // Utilitário para reverter uma string
    int i = 0, j = str_len - 1;
    if(str_len <= 0) return str;
    char *temp = (char*) malloc((str_len + 1) * sizeof(char));
    strcpy(temp, str);
    while(i < j){
        swap(temp[i], temp[j]);
        i++, j--;
    }
    return temp;
}

// Função utilizada para comparar dois caracteres, ignorando a diferença em casing de acordo com a flag ignore_case
bool isEqual(char a, char b, bool ignore_case){
    if(ignore_case){
        a = tolower(a);
        b = tolower(b);
    }
    return (a == b);
}

// Funcao utilizada no wu manber para tratar casos com ignore_case = true
int isLatin(char c){
    if(c >= 'a' && c <= 'z') return c;
    else if(c >= 'A' && c <= 'Z') return c + 32;
    else return -1;
}

// Cria a máscara de caracteres para o wu-manber
long long *char_mask(long long *C,char *patt, int patt_size){
    long long pos_mask = (1 << patt_size) - 2; 

    C = (long long*) malloc(alpha_len * sizeof(long long)); 

    for(int i = 0; i < alpha_len; i++) {
        C[i] = (1 << patt_size) - 1;
    }

    for(int j = 0; j < patt_size; j++){
        int letter = isLatin(patt[j]);
        if(ignore_case && letter != -1){
            C[letter] = C[letter] & pos_mask;
        }
        else{
            C[patt[j]] = C[patt[j]] & pos_mask;
        }

        pos_mask = (pos_mask << 1LL) | 1LL;
    }

    return C;
}

// Inicializa os pulos de acordo com a borda estrita
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

// Inicializa array do mau caractere
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

// Constroi a Trie
void build_go_to(vector<char*> &pat_set, vector<int> &patt_size, int qnt_pat){
    int nxt = 1;
    vector<vector<int>> temp_occ,temp_go_to; // Vetores dinâmicos temporários para preparar a Trie

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

    // Conversão de vector para ponteiro
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

        }
    }
}

// Constroi o fail da trie
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
// Inicia o preprocessamento dos padrões em todos os algoritmos utilizados
void preprocess(Args &pmt,vector<vector<int>> &alg_used){

    strict_nxt = (int**) malloc((pmt.num_patt) * sizeof(int*));
    bad_char = (int**) malloc((pmt.num_patt) * sizeof(int*));
    good_suffix = (int**) malloc((pmt.num_patt) * sizeof(int*));
    C_wu = (long long **) malloc((pmt.num_patt) * sizeof(long long*));
    // Se a flag ignore_case estiver ligada, transformamos todos os padrões em lowercase
    if(ignore_case){
        for(int i = 0; i < pmt.num_patt; i++){
            for(int j = 0; j < pmt.patt_size[i]; j++){
                pmt.patterns[i][j] = tolower(pmt.patterns[i][j]);
            }
        }
    }
    // Se o algoritmo utilizado for aho-corasick
    if(alg_used[0][0] == ALG_AHO_CORASICK){
        build_go_to(pmt.patterns, pmt.patt_size, pmt.num_patt);
        build_failure(aho_go_to);
    }
    else{
        for(int i = 0; i < pmt.num_patt; i++){
            bool k = false, bm = false, wm = false;
            for(auto &txt: alg_used){ // Verifica o algoritmo utilizado
                switch(txt[i]){
                    case ALG_KMP:
                        k = true;
                        break;
                    case ALG_BOYER_MOORE:
                        bm = true;
                        break;
                    case ALG_WU_MANBER:
                        wm = true;
                        break;
                }
            }
            // Faz o preprocessamento de acordo com o algoritmo utilizado naquele padrão
            if(k or bm) strict_nxt[i] = init_strict_next(pmt.patterns[i], pmt.patt_size[i], ignore_case, strict_nxt[i]);    
            if(bm){
                bad_char[i] = init_bad_char(pmt.patterns[i], pmt.patt_size[i], ignore_case, bad_char[i]);
                good_suffix[i] = init_good_suffix(pmt.patterns[i], pmt.patt_size[i], ignore_case, good_suffix[i],strict_nxt[i]);
            }
            else if(wm){
                C_wu[i] = char_mask(C_wu[i], pmt.patterns[i], pmt.patt_size[i]);
            }
        }
    }
}