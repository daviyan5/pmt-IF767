#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

//Functions: badchar, good_suffix_bf, good_suffix, boyermoore, init_next

void bad_char(char *pat, char *alphabet){
    int l = strlen(alphabet);
    int m = strlen(pat);

    //fazer um map de alphabet[i] para i ?? 
    //C = {alphabet[i]:-1 for i in range(l)}
    for(int i = 0; i < m; i++){
        C[pat[i]] = i;
    }

    return C;
}

void reverse_string(char *txt, int begin, int end){
    if(begin >= end) return;

    char c = *(txt + begin);
    *(txt + begin) = *(txt + end);
    *(txt + end) = c;

    reverse_string(txt, ++begin, --end);
}

void good_suffix_bf(char *pat){
    int m = strlen(pat);
    int delta[m + 1] = {};

    for(int j = -1; j < m; j++){
        //U = pat[j + 1 : ]
        int len_U = m - 1 - j;
        for(int k = m - 1; k > -1; k--){
            //if (len_U <= k and U == pat[k-len_U:k]) or \
                (k < len_U and pat[:k] == pat[m-k:]):
             //   break
        }
        delta[j] = k;
    }

    //return [m - d for d in delta]
}

int* good_suffix(char *pat){
    int m = strlen(pat);

    int *Pi = init_next(pat);

    reverse_string(pat, 0, m - 1);                //reverse the string

    int *PiR = init_next(pat);                   // init_next ta no kmp

    reverse_string(pat, 0, m - 1);                //reverse to the original

    int S[m + 1];
    for(int i = 0; i < m + 1; i++) {
        S[i] = m - Pi[m];
    }

    for(int l = 1; l < m; l++){
        int j = (m - 1) - PiR[l];
        if(l - PiR[l] < S[j]){
            S[j] = l - PiR[l];
        }
    }

    return S;
}


void boyer_moore(char *txt, char *pat, char *alphabet, int *bc = NULL, int *gs = NULL){
    //occ = []
    int n = strlen(txt);
    int m = strlen(pat);

    if(bc == NULL) C = bad_char(pat, alphabet);
    else C = bc;

    if(gs == NULL) S = good_suffix_bf(pat);
    else S = gs;

    int i = 0, j;
    while(i <= n - m){
        j = m - 1;
        while(j >= 0 && txt[i + j] == pat[j]) j--;

        if(j < 0){
            //occ.append(i)
            i += S[j]
        }
        else{
            i += max(S[j], j - C[txt[i + j]]);
        }
    }

    //return occ
}

int main(){

}