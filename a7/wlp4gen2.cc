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

struct tree {
    string production;
    vector <string> tokens;
    vector <tree *> children;
    string type; 
    ~tree() {
        for(int i = 0; i  < (int) children.size() ; i++) {
            delete children[i];
        }
    }
} ;

bool check_terminal(string s) {
    set<string> terminals = { "BOF", "BECOMES", "COMMA", "ELSE", "EOF", "EQ",  "GE", "GT", "ID", "IF", "INT",
                              "LBRACE", "LE", "LPAREN", "LT", "MINUS", "NE", "NUM", "PCT", "PLUS", "PRINTLN",
                               "RBRACE", "RETURN", "RPAREN", "SEMI", "SLASH", "STAR", "WAIN", "WHILE", "AMP", "LBRACK", 
                               "RBRACK", "NEW", "DELETE", "NULL" };
    if (terminals.count(s) == 0) {
        return false;
    } else {
        return true;
    }
}

void buildptree (struct tree *ptree, string inputs) {
    string inputline;
    if(inputs == " "){
        getline(cin, inputline);
    } else {
        inputline = inputs;
    }
    ptree->production = inputline;
    istringstream ss(inputline);
    string token;
    while(ss >> token) {
        ptree->tokens.push_back(token);
    }
    string lhs;
    lhs = ptree->tokens[0];
    if(!check_terminal(lhs)) {
        for(int i =  1; i < (int) ptree->tokens.size(); i++) {
             struct tree *ptree1 = new struct tree(); 
             buildptree(ptree1, " ");
             ptree->children.push_back(ptree1);
        }
    }
}

void getSymtable(map<string, string> &table1, struct tree *ptree, vector<pair<string, string> > &vtable1) {
    if((ptree->tokens.size() == 3) && (ptree->tokens[0] == "dcl") && (ptree->tokens[1] == "type")&&(ptree->tokens[2] == "ID")){
        string stype;
        string svalue;
        if (ptree->children[0]->tokens.size() == 3) {
            stype = "int*";
        }else {
            stype = "int";
        }
        svalue = ptree->children[1]->tokens[1];
        if(table1.find(svalue) == table1.end()) {
            table1.insert(make_pair(svalue,stype)); 
            vtable1.push_back(make_pair(svalue,stype));
        } else {
              throw string("ERROR: duplicate dcl" );
        }
    } else {
        for(int i = 0; i < (int) ptree->children.size(); i++) {
           getSymtable(table1, ptree->children[i], vtable1);
        }
    }
}

void printSymtable(vector<pair<string, string> > vtable1, string pname, int paran) {
    if(pname == "main") {
        cerr << "wain: ";
        cerr << vtable1[0].second << " ";
        if (vtable1[1].second != "int") {
            throw string( "ERROR: the second para must be int" );
        }
        cerr << vtable1[1].second << endl;
    } else {
        cerr <<pname << ": ";
        for(int i = 0; i < paran; i ++) {
            cerr << vtable1[i].second << " ";
        }
        cerr << endl;
    }
    for (int i = 0; i < (int)vtable1.size(); i++) {
        cerr << vtable1[i].first << " " << vtable1[i].second<< endl;
    }
} 

void check_type(struct tree * ptree, map<string, string> table1, map<string, pair<vector<string>, map<string, string> > >alltable, string pname) {
    if ((ptree->production == "factor ID")|| (ptree->production == "lvalue ID")) {
        string svalue =  ptree->children[0]->tokens[1];
        if(table1.find(svalue) == table1.end()) {
            throw string( "ERROR: variable not decleared");
        }
    }
    if((ptree->production == "factor ID LPAREN RPAREN") || (ptree->production == "factor ID LPAREN arglist RPAREN")) {
        string s = ptree->children[0]->tokens[1];
        if((s != pname) && (alltable.find(s) == alltable.end())) {
            throw string( "ERROR: procedure not decleared");
        }
    }
    for(int i = 0; i < (int)ptree->children.size(); i++) {
        check_type(ptree->children[i],table1,alltable,pname);
    }
    
}

int count_para(struct tree *ptree) {
    int paran = 0;
    if((ptree->production == "paramlist dcl")  || (ptree->production == "paramlist dcl COMMA paramlist")) {
        paran++;
    }
    for(int i = 0; i < (int) ptree->children.size(); i ++) {
        paran += count_para(ptree->children[i]);
    }
    return paran;
}

int main () { 
    vector<tree*> alltr;
    map<string, pair<vector<string>, map<string, string> > > alltable;
    try {
        string inputline;
        getline(cin, inputline); // start BOF procedures EOF
        getline(cin, inputline); // BOF BOF
        while (true) {
            getline(cin, inputline);
            if(inputline == "EOF EOF") {
                break;
            }
            if(inputline == "procedures main") {   //case 1
                 struct tree *ptree = new struct tree();
                buildptree (ptree, "procedures main");
                alltr.push_back(ptree);
                if(alltable.find("main") != alltable.end()) {
                    throw string("ERROR: duplicate main");
                }
                map<string, string> table1;
                vector<pair<string, string> > vtable1;
                getSymtable(table1, ptree, vtable1);
                check_type(ptree,table1, alltable, " ");
                printSymtable(vtable1, "main", 2);
                vector<string> params;
                params.push_back(vtable1[0].second);
                params.push_back(vtable1[1].second);
                alltable.insert(make_pair("main", make_pair(params,table1)));
            }
            if(inputline == "procedure INT ID LPAREN params RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE") {
                 struct tree *ptree = new struct tree();
                buildptree (ptree, "procedure INT ID LPAREN params RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE");
                alltr.push_back(ptree);
                string pname = ptree->children[1]->tokens[1];
                if(alltable.find(pname) != alltable.end()) {
                    throw string("ERROR: duplicate procedure name");
                }
                map<string, string> table1;
                vector<pair<string, string> > vtable1;
                getSymtable(table1, ptree, vtable1);
                check_type(ptree,table1,alltable, pname);
                int paran = 0;
                paran = count_para(ptree);
                printSymtable(vtable1, pname, paran);
                vector<string> params;
                for(int i = 0; i < paran; i++) {
                    params.push_back(vtable1[i].second);
                }
                alltable.insert(make_pair(pname, make_pair(params,table1)));
            }
        }
    } catch(string msg) {
    cerr << msg << endl;
    }
    for(int i = 0; i < (int)alltr.size(); i++) {
        delete alltr[i];
    }
}