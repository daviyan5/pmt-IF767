#include "bits/stdc++.h"
using namespace std;

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);
    int seed = 42;
    srand(seed);
    ifstream input;
    input.open("patterns.txt");

    int words_qnt;
    input >> words_qnt;

    vector<string> words;

    for(int i = 0; i < words_qnt; i++){
        string pat;
        input >> pat;
        words.push_back(pat);
    }

    vector<char> letters(52);
    for(int i = 0, c = 'a'; i < 26; i++, c++) letters[i] = c;
    for(int i = 26, c = 'A'; i < 52; i++, c++) letters[i] = c;

    int mod = words_qnt + 54;
    int mul[] = {1,2,4,20,40};
    for(int i = 0; i < 5; i++){
        cout << "Criando arquivo" << " " << i << endl;
        int num_lines = 1111111 * mul[i];
        string s = "text_";
        s.push_back(i + '0');
        s +=".txt";
        ofstream out;
        out.open(s);
        for(int i = 0; i < num_lines; i++){
            int what = rand() % mod;

            if(what < words_qnt){
                if(i % 100 == 0){
                    string aux = words[what];
                    int distance = rand() % 10;
                    while(distance--){
                        int pos = rand() % words[what].length();
                        int new_letter = 33 + rand() / (RAND_MAX / (126 - 33 + 1) + 1);
                        aux[pos] = char(new_letter);
                    }
                    out << aux;
                }
                else out << words[what];
            }
            else if(what == words_qnt) out << ' ';
            else if(what == words_qnt + 1) out << '\n';
            else out << letters[what - words_qnt - 2] << '\n';
        }
    }
    

}