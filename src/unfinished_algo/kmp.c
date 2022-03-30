#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

int* init_next_bf(char *pat){
    int m = strlen(pat);
    int nxt[m + 1];
    memset(nxt, -1, sizeof(nxt));

    for(int j = 1; j < m + 1; j++){
        for(int k = 0; k < j; k++){
            if(pat[:k] == pat[j-k : j]){         //Vamos fazer um hash?
                nxt[j] = k;
            }
        }
    }

    return nxt;
}

int* init_next(char *pat){
    int m = strlen(pat);
    int nxt[m + 1];

    memset(next, 0, sizeof(nxt));

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

int* init_strict_next(char *pat){
    int m = strlen(pat);
    int nxt[m + 1];
    memset(nxt, -1, sizeof(nxt));

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

int* kmp(char *txt, char *pat, int *nxt = NULL){
    //occ = []
    int n = strlen(txt);
    int m = strlen(pat);

    if(nxt == NULL) nxt = init_next(pat);   

    int i = 0, j = 0;
    while(i <= n - m){
        while(j < m && txt[i + j] == pat[j]) j++;

        if(j == m){
            //occ.append(i);
        }
        else continue;

        i = i + (j - nxt[j]);
        j = max(0, nxt[j]);
    }

    //return occ
}

int main(){

}