#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <stdlib.h>
#include <map>
#include <string>
#include <math.h> 
using namespace std;

int main() {
    int num;
    string s;
    vector<string> alphabet, states, finish_states, input;
    string initial_state;
    map<pair<string, string>,string> transitions;
    
    cin >> num; 
    for(int i = 0; i < num; i++) {
        cin >> s;
        alphabet.push_back(s);
    }

    cin >> num; 
    for(int i = 0; i < num; i++) {
        cin >> s;
        states.push_back(s);
    }

    cin >> initial_state;
    cin >> num;
    for(int i = 0; i < num; i++) {
        cin >> s;
        finish_states.push_back(s);
    }

    cin >> num;
    string lineStr;
    getline(cin, lineStr);
    for(int i = 0; i < num; i++) {
        getline(cin, lineStr);
        std::stringstream line(lineStr);
        std::string s1, s2, s3;
        if(line >> s1 >> s2 >> s3) {
            pair<string, string> trans = make_pair(s1,s2);
            transitions.insert(make_pair(trans, s3));
        }
    }
    while(getline(cin, s)) {
        std::stringstream line(s);
        string s1;
        bool if_found = false;
        bool if_no_error = true;
        string initial_state2 = initial_state;
        while(line >> s1) {
            pair<string, string> trans = make_pair(initial_state2,s1);
            if(transitions.find(trans) == transitions.end()){
                if_no_error = false;
                break;
            } else {
                initial_state2 = transitions[trans];
            }
        }
        for(int i = 0; i < (int)finish_states.size(); i++) {
            if(finish_states[i] == initial_state2) {
                if_found = true;
            }
        }  
        if((if_found) && if_no_error){
           cout << "true" << endl;
        } else {
           cout << "false" << endl;
        }
    } 
}