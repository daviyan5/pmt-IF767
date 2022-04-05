#ifndef H_PRE_PROCESS
#define H_PRE_PROCESS
#include "algorithms.hpp"


extern int **strict_nxt, **bad_char, **good_suffix;
extern int **aho_occ, *aho_qnt_occ;
extern int **aho_go_to, *aho_fail;
extern int size_goto, occ_size;
extern long long **C_wu;

// Tamanho do alfabeto considerado (128 padrão)
const int alpha_len = 128;

int isLatin(char c);
bool isEqual(char a, char b, bool ignore_case);
void preprocess(Args &pmt,vector<vector<int>> &alg_used);


struct FilaNode{
    int val;
    FilaNode* next;
    FilaNode(){
        val = -1, next = nullptr;
    }
    FilaNode(int i){
        val = i, next = nullptr;
    }
};

    
class Fila{
public:
    Fila(){
        size = 0;
        head = nullptr;
        rear = nullptr;
    }
    void push(int val){
        size += 1;
        if(!rear){
            head = new FilaNode(val);
            rear = head;
        }
        else{
            rear->next = new FilaNode(val);
            rear = rear->next;
        }
    }
    int pop(){
        if (size == 0) return -1;
        size -= 1;
        int temp_val = head->val;
        FilaNode *temp = head;
        head = head->next;
        if (!head) rear = nullptr;
        delete[] temp;
        return temp_val;
    }
    int front(){
        return size > 0? head->val : -1;
    }
    bool empty(){
        return size == 0;
    }
    void free_all(){
        struct FilaNode* tmp;
        while(head){
            tmp = head;
            head = head->next;
            delete[] tmp;
        }
        delete[] head;
        delete[] rear;
    }
private:
    int size;
    FilaNode *head, *rear;
};


#endif 