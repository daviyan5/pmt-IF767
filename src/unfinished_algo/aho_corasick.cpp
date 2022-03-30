#include "bits/stdc++.h"
using namespace std;

int char_index(char *alphabet, char c){
    //to do
}

// goto[s][c]

void build_go_to(char *pat_set, char *alphabet){
    int l = strlen(alphabet);
    int len = strlen(pat_set);
    // go_to = [l*[-1]]
    // occ = [[]]
    int nxt = 1;
    for(int k = 0, k < len; k++){
        char *pat = pat_set[k];
        int m = strlen(pat);
        int cur = 0, j = 0;
        int c = char_index(alphabet, pat[j]);

        while(j < m && go_to[cur][c] != -1){
            cur = go_to[cur][c];
            j++;
            c = char_index(alphabet, pat[j]);
        }

        while(j < m){
            c = char_index(alphabet, pat[j]);
            go_to[cur][c] = nxt;
            //go_to.append(l*[-1])
            //occ.append([])
            cur = nxt;
            j++;
            nxt++;
        }
        //occ[cur].append(k)
    }

    for(int c = 0; c < l; c++){
        if(go_to[0][c] == -1){
            go_to[0][c] = 0;
        }
    }

    //return (go_to, occ);
}

void build_fail(char *pat_set, char *alphabet, int **go_to, occ = ascii){
    int n = strlen(go_to);
    int l = strlen(alphabet);

    int fail[n];
    memset(fail, -1, sizeof(fail));

    queue<int> bfs;
    for(int c = 0; c < l; c++){
        if(go_to[0][c] > 0){
            bfs.push(go_to[0][c]);
            fail[go_to[0][c]] = 0;
        }
    }

    while(!bfs.empty()){
        cur = bfs.front();
        bfs.pop();
        for(int c = 0; c < l; c++){
            suc = go_to[cur][c];
            if(suc >= 0){
                bfs.push(suc);
                brd = fail[cur];
                while(go_to[brs][c] < 0){
                    brd = fail[brd];
                }
                fail[suc] = go_to[brd][c]
                //occ[suc].extend(occ[fail[suc]])
            }
        }
    }

    //return (fail, occ);
}

void build_fsm(char *pat_set, char *alphabet){
    // goto, occ = build_goto(pat_set, ab)
    // fail, occ = build_fail(pat_set, ab, goto, occ)
    // return (goto, fail, occ)
}

void aho_corasick(char *txt, char *alpha, fsm = NULL){
    if(!fsm){
        fsm = build_fsmi(pat_set, alphabet);
    }
    //(goto, fail, occ) = fsm
    //pat_lens = [len(p) for p in pat_set]
    //occ_set = [[] for p in pat_set]

    int cur = 0, i = 0;
    int n = strlen(txt);

    while(i < n){
        int c = char_index(alphabet, txt[i]);
        while(go_to[cur][c] < 0){
            cur = fail[cur];
        }
        cur = go_to[cur][c];
        // for k in occ[cur]:
        //     occ_set[k].append(i - pat_lens[k] + 1)
        i++;
    }

    //return occ_set;
}


int main(){

}