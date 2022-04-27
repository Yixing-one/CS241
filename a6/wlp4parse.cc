#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <sstream>
#include <map>
#include <set>
#include <cstdint>
#include <ostream>
#include <stack>
#include <fstream> 
using namespace std;


struct parsetree {
    string node;
    vector<parsetree *> children;

    ~parsetree() {
        for(vector<parsetree*>::iterator it=children.begin(); it != children.end(); it++) { 
            delete (*it);
        }
        children.clear();
    }
};

void print(struct parsetree *pr) {
    cout << pr->node << endl;
    for(int i = 0; i < (int)pr->children.size(); i++) {
        print(pr->children[i]);
    }
}


void recognize(map<pair<int, string>,pair<string, int> > derivation, vector<string> procedures, int endPro) {
    struct vector<parsetree *> output;
    int reducetotal = 0;
    vector<pair<string, string> > unreads;
    vector<string> reads;
    vector<int> state;
    string s;
    string s1;
    string ss1;
    int inputSize;
    int state1 = 0;
    int currentUnreads = 0;
    int currentState  = 0;
    int currentReads = 0;
    unreads.push_back(make_pair("BOF","BOF"));
    while (getline(cin, s)) {
        istringstream stream(s);
        stream >> s1;
        stream >> ss1;
        unreads.push_back(make_pair(s1, ss1));
    }
    unreads.push_back(make_pair("EOF","EOF"));
    inputSize = unreads.size();
    state.push_back(0);
    while(currentUnreads < inputSize) {
       pair<int, string> key = make_pair(state1, unreads[currentUnreads].first);
      // cerr << unreads[currentUnreads]<<endl;
       if(derivation.count(key) > 0) {
          pair<string, int> value = derivation[key];
          if(value.first == "shift") {
              state.push_back(value.second);
              reads.push_back(unreads[currentUnreads].first);
              if(unreads[currentUnreads].second != "") {
                  string s5 = unreads[currentUnreads].first + " " + unreads[currentUnreads].second;
                  struct parsetree *t1 = new struct parsetree();
                  t1->node = s5;
                  output.push_back(t1);
              }
              state1 = value.second;
              currentUnreads++;
              currentReads++;
              currentState++;
          } else if(value.first == "reduce") {
              int numPro = value.second;
              string pro = procedures[numPro];
              istringstream stream(pro);
              string left;
              string right;
              stream >> left;
              struct parsetree *t2 = new struct parsetree();
              t2->node = pro;
              int k = 0;
              while (stream >> right) {
                 k++;
                 reads.pop_back();
                 state.pop_back();
                 currentReads--;
                 currentState--;
                 reducetotal += 1;
              }
              for(int i = 0; i < k; i ++) {
                 int l = output.size()-k+i;
                 t2->children.push_back(output[l]);
              }
              for(int i = 0; i < k; i ++) {
                 output.pop_back();
              }
              output.push_back(t2);             
                currentUnreads--;
                unreads[currentUnreads].first = left;
                unreads[currentUnreads].second = "";
                state1 = state[currentState];
            } else {
                for(int i = 0; i < (int)output.size(); i ++){
                  delete (output[i]);
              }
              throw string("ERROR: must be shift or reduce");
              return;
          }
       } else {
           stringstream stream;
           stream << currentUnreads;
           for(int i = 0; i < (int)output.size(); i ++){
                  delete (output[i]);
           }
           throw string("ERROR at " + stream.str());
           return;
       }
    }
    struct parsetree *t3 = new struct parsetree();
    t3->node = procedures[endPro];
    for(int i = 0; i < (int)output.size();i++) {
        t3->children.push_back(output[i]);
    }
    print(t3);
    delete t3;
    output.clear();
}

int main(){
    ifstream in("wlp4");
    vector<string> terminals;
    vector<string> nonterminals;
    string start;
    vector<string> procedures;
    map<pair<int, string>,pair<string, int>> derivation;
    
    
    //read terminals
    int n;
    string s;
    getline(in, s);
    istringstream iss(s);
    iss >> n;
    for(int i = 0; i < n; i++) {
        getline(in, s);
        terminals.push_back(s);
    }
    terminals.push_back("EOF");
    terminals.push_back("BOF");

    //read nonterminals
    getline(in, s);
    istringstream iss1(s);
    iss1 >> n;
    for(int i = 0; i < n; i++) {
        getline(in, s);
        nonterminals.push_back(s);
    }

    //read start
    getline(in, start);

    //read procedure
    getline(in, s);
    istringstream iss2(s);
    iss2 >> n;
    for(int i = 0; i < n; i++) {
        getline(in, s);
        procedures.push_back(s);
    }

    getline(in, s);
    istringstream iss3(s);
    iss3 >> n;
    getline(in, s);
    istringstream iss4(s);
    iss4 >> n;
    for(int i = 0; i < n; i++) {
        int stateNum1;
        in >> stateNum1;
        string inputS1;
        string inputS2;
        in >> inputS1;
        in >> inputS2;
        int stateNum2;
        in >> stateNum2;
        derivation.insert(make_pair(make_pair(stateNum1, inputS1), make_pair(inputS2, stateNum2)));
    }
    try {
        int endPro = -1;
        for(int i = 0; i < (int) procedures.size(); i++) {
            istringstream iss5(procedures[i]);
            string s3;
            iss5 >> s3;
            if(s3 == start) {
               endPro = i;
            }
        }
        recognize(derivation, procedures, endPro);
    } catch (string &err) {
        cerr << err << endl;
        return 1;
    }
}