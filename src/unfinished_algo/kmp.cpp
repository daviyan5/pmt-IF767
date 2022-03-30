#include "bits/stdc++.h"
using namespace std;

bool cmp_string(char *pat, int i, int j, int k){
    for(int x = i, y = j - k; x < i + k; x++, y++){
        if(pat[x] != pat[y]) return false;
    }

    return true;
}

vector<int> init_next_bf(char *pat){
    int m = strlen(pat);
    vector<int> nxt(m + 1, -1);

    for(int j = 1; j < m + 1; j++){
        for(int k = 0; k < j; k++){
            if(cmp_string(pat, 0, j, k)){   //Vamos fazer um hash?
                nxt[j] = k;
            }
        }
    }

    return nxt;
}

vector<int> init_next(char *pat){
    int m = strlen(pat);
    vector<int> nxt(m + 1, 0);

    nxt[0] = -1;

    int i = 1, j = 0;
    while(i + j < m){
        while(i + j < m && pat[i + j] == pat[j]){
            j++;
            next[i + j] = j;
        }

        i += (j - nxt[j]);
        j = max(0, nxt[j]);
    }

    return nxt;
}

vector<int> init_strict_next(char *pat){
    int m = strlen(pat);
    vector<int> nxt(m + 1, -1);

    if(m == 1 || (m > 1 && pat[0] != pat[1])) nxt[1] = 0;

    int i = 1, j = 0;
    while(i + j < m){
        while(i + j < m && pat[i + j] == pat[j]){
            j++;
            if(i + j == m || pat[i + j] != pat[j]) nxt[i + j] = j;
            else nxt[i + j] = nxt[j];
        }

        if(j == 0 && ((i + 1 == m) || (pat[0] != pat[i + 1]))){
            nxt[i + 1] = 0;
        }

        i += (j - nxt[j]);
        j = max(0, nxt[j]);
    }

    return nxt;
}

vector<int> kmp(char *txt, char *pat, int *nxt = NULL){
    vector<int> occ;
    int n = strlen(txt);
    int m = strlen(pat);

    if(nxt == NULL) nxt = init_next(pat);   

    int i = 0, j = 0;
    while(i <= n - m){
        while(j < m && txt[i + j] == pat[j]) j++;

        if(j == m) occ.push_back(i);
        else continue;

        i = i + (j - nxt[j]);
        j = max(0, nxt[j]);
    }

    return occ;
}

int main(){

}