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
using namespace std;


void recognize(map<pair<int, string>,pair<string, int>> derivation, vector<string> procedures, int endPro) {
    vector<string> unreads;
    vector<string> reads;
    vector<int> state;
    string s;
    string s1;
    int inputSize;
    int state1 = 0;
    int currentUnreads = 0;
    int currentState  = 0;
    int currentReads = 0;
    while (getline(cin, s)) {
        istringstream stream(s);
        while(stream >> s1) {
            unreads.push_back(s1);
        }
    }
    inputSize = unreads.size();
    state.push_back(0);
    while(currentUnreads < inputSize) {
       pair<int, string> key = make_pair(state1, unreads[currentUnreads]);
       if(derivation.count(key) > 0) {
          pair<string, int> value = derivation[key];
          if(value.first == "shift") {
              state.push_back(value.second);
              reads.push_back(unreads[currentUnreads]);
              state1 = value.second;
              currentUnreads++;
              currentReads++;
              currentState++;
          } else if(value.first == "reduce") {
              int numPro = value.second;
              string pro = procedures[numPro];
              cout << pro << endl;
              istringstream stream(pro);
              string left;
              string right;
              stream >> left;
              while (stream >> right) {
                 reads.pop_back();
                 state.pop_back();
                 currentReads--;
                 currentState--;
              }
                currentUnreads--;
                unreads[currentUnreads] = left;
                state1 = state[currentState];
            } else {
              throw string("ERROR: must be shift or reduce");
          }
       } else {
           stringstream stream;
           stream << currentUnreads + 1;
           throw string("ERROR at " + stream.str());
       }
    }
    cout << procedures[endPro] << endl;
}

int main(){
    vector<string> terminals;
    vector<string> nonterminals;
    string start;
    vector<string> procedures;
    map<pair<int, string>,pair<string, int>> derivation;
    
    //read terminals
    int n;
    string s;
    getline(cin, s);
    istringstream iss(s);
    iss >> n;
    for(int i = 0; i < n; i++) {
        getline(cin, s);
        terminals.push_back(s);
    }

    //read nonterminals
    getline(cin, s);
    istringstream iss1(s);
    iss1 >> n;
    for(int i = 0; i < n; i++) {
        getline(cin, s);
        nonterminals.push_back(s);
    }

    //read start
    getline(cin, start);

    //read procedure
    getline(cin, s);
    istringstream iss2(s);
    iss2 >> n;
    for(int i = 0; i < n; i++) {
        getline(cin, s);
        procedures.push_back(s);
    }

    getline(cin, s);
    istringstream iss3(s);
    iss3 >> n;
    getline(cin, s);
    istringstream iss4(s);
    iss4 >> n;
    for(int i = 0; i < n; i++) {
        int stateNum1;
        cin >> stateNum1;
        string inputS1;
        string inputS2;
        cin >> inputS1;
        cin >> inputS2;
        int stateNum2;
        cin >> stateNum2;
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