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

string getType(struct tree * ptree, map<string, string> table1, map<string, pair< vector<string>, map<string, string> > > alltable) {
    string production = ptree->production;
    string check;
    string check1;
    if(ptree->tokens[0] == "NUM") {
        return "int";
    }
    if(ptree->tokens[0] == "ID") {
        return table1[ptree->tokens[1]];
    }
    if(ptree->tokens[0] == "NULL") {
        return "int*";
    }
    //procedures → procedure procedures
    if(production == "procedures procedure procedures") {
        check = getType(ptree->children[0], table1, alltable);
        check = getType(ptree->children[1], table1, alltable);
    }
    //procedures → main
    if(production == "procedures main") {
        check = getType(ptree->children[0], table1, alltable);
    }
    //procedure → INT ID LPAREN params RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE 
    if (production == "procedure INT ID LPAREN params RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE") {
        check = getType(ptree->children[6], table1, alltable);
        check = getType(ptree->children[7], table1, alltable);
        if(getType(ptree->children[9], table1, alltable) != "int")  {
            throw string ("ERROR: return value must be int");
        } 
    }
    //main → INT WAIN LPAREN dcl COMMA dcl RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE 
    if (production == "main INT WAIN LPAREN dcl COMMA dcl RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE") {
        check = getType(ptree->children[8], table1, alltable);
        check = getType(ptree->children[9], table1, alltable);
        if(getType(ptree->children[11], table1, alltable) != "int")  {
            throw string ("ERROR: return value must be int");
        } 
    }
    //params →
    if(production == "params") {
    }
    //params → paramlist
    if(production == "params paramlist") {
        check = getType(ptree->children[0], table1, alltable);
    }
    //paramlist → dcl
    if(production == "paramlist dcl") {
        check = getType(ptree->children[0], table1, alltable);
    }
    //paramlist → dcl COMMA paramlist
    if(production == "paramlist dcl COMMA paramlist") {
        check = getType(ptree->children[0], table1, alltable);
        check = getType(ptree->children[2], table1, alltable);
    }
    //type → INT
    if(production == "type INT") {
        return "int";
    }
    //type → INT STAR
    if(production == "type INT STAR") {
        return "int*";
    }
    // dcls ->
    if(production == "dcls") {
    }
    //dcls → dcls dcl BECOMES NUM SEMI
    if (production == "dcls dcls dcl BECOMES NUM SEMI") {
        check = getType(ptree->children[0], table1, alltable);
        if(getType(ptree->children[1], table1,alltable)  != "int")  {
            throw string ("ERROR: can only sign num to int variable");
        }   
    }
    //dcls → dcls dcl BECOMES NULL SEMI
    if (production == "dcls dcls dcl BECOMES NULL SEMI") {
        check = getType(ptree->children[0], table1, alltable);
        if(getType(ptree->children[1], table1, alltable) != "int*")  {
            throw string ("ERROR: can only sign NULL to int* variable");
        }
    }
    //dcl → type ID
    if(production ==  "dcl type ID") { 
         check = getType(ptree->children[0], table1, alltable);
         check1 = getType(ptree->children[1], table1, alltable);
         if(check != check1){
             throw string ("ERROR: invalid type for variable");
         }
         return check;
     }
     //statements →
     if(production == "statements") {
    }
    //statements → statements statement
    if(production == "statements statements statement") {
        check = getType(ptree->children[0], table1, alltable);
        check = getType(ptree->children[1], table1, alltable);
    }  
    //statement → lvalue BECOMES expr SEMI
    if (production == "statement lvalue BECOMES expr SEMI") {
       check = getType(ptree->children[0], table1, alltable);
       check1 = getType(ptree->children[2], table1, alltable);
       if(check != check1)  {
            throw string ("ERROR: assign value must be the same type");
        }   
    }
    //statement → IF LPAREN test RPAREN LBRACE statements RBRACE ELSE LBRACE statements RBRACE 
    if (production == "statement IF LPAREN test RPAREN LBRACE statements RBRACE ELSE LBRACE statements RBRACE") {
       check = getType(ptree->children[2], table1, alltable);
       check = getType(ptree->children[5], table1, alltable);
       check = getType(ptree->children[9], table1, alltable);  
    }
    //statement → WHILE LPAREN test RPAREN LBRACE statements RBRACE 
    if (production == "statement WHILE LPAREN test RPAREN LBRACE statements RBRACE") {
       check = getType(ptree->children[2], table1, alltable);
       check = getType(ptree->children[5], table1, alltable);  
    }
    //statement → PRINTLN LPAREN expr RPAREN SEMI
     if (production == "statement PRINTLN LPAREN expr RPAREN SEMI") {
       check = getType(ptree->children[2], table1, alltable);
       if(check != "int") {
           throw string ("ERROR: only int can be print");
       }
    }
    //statement → DELETE LBRACK RBRACK expr SEMI
    if (production == "statement DELETE LBRACK RBRACK expr SEMI") {
        check = getType(ptree->children[3], table1, alltable);
        if(check != "int*")  {
            throw string ("ERROR: can only delete int*");
        }
    }
    //test → expr EQ expr  
    //test → expr NE expr  
    //test → expr LT expr 
    //test → expr LE expr  
    //test → expr GE expr  
    //test → expr GT expr 
     if (production == "test expr EQ expr" || production == "test expr NE expr"
        || production == "test expr LT expr" || production == "test expr LE expr"
        || production == "test expr GE expr" || production == "test expr GT expr") {
        check = getType(ptree->children[0], table1, alltable);
        check1 = getType(ptree->children[2], table1, alltable);
        if ((check != check1)||(check1 != "int")) {
          throw string("ERROR: comparisonn fails");
        }
        return "int";
     }
     //expr → term
     if (production == "expr term") {
        return getType(ptree->children[0], table1, alltable);
     }
     //expr → expr PLUS term
     if(production == "expr expr PLUS term") {
        check = getType(ptree->children[0], table1, alltable);
        check1 = getType(ptree->children[2], table1, alltable);
        if((check == "int") && (check1 == "int")){
            return "int";
        } else if ((check == "int*") && (check1 == "int")) {
            return "int*";
        } else if ((check == "int") && (check1 == "int*")) {
            return "int*";
        } else {
            throw string ("ERROR:invalid types for plus");
        }
    }
    //expr → expr MINUS term 
    if(production == "expr expr MINUS term") {
        check = getType(ptree->children[0], table1, alltable);
        check1 = getType(ptree->children[2], table1, alltable);
        if((check == "int") && (check1 == "int")){
            return "int";
        } else if ((check == "int*") && (check1 == "int")) {
            return"int*";
        } else if ((check == "int*") && (check1 == "int*")) {
            return "int";
        } else {
            cerr << check << endl;
            cerr << check1<< endl;
            throw string ("ERROR:invalid types for minus");
        }
    }
    //term → factor 
    if (production == "term factor") {
        return getType(ptree->children[0], table1, alltable);
    }
    //term → term STAR factor 
    //term → term SLASH factor 
    //term → term PCT factor
    if (production ==  "term term STAR factor" || production == "term term SLASH factor"
        || production == "term term PCT factor" ) {
            check = getType(ptree->children[0], table1, alltable);
            check1 = getType(ptree->children[2], table1, alltable);
            if((check != "int") || (check1 != "int")){
                    throw string("ERROR :invalid operation, for * / %, both value must be int");
            }
            return "int";
    }
    //factor → ID 
    if (production == "factor ID") {
        return getType(ptree->children[0], table1, alltable);
    }
    //factor → NUM  
    if (production == "factor NUM") {
        return getType(ptree->children[0], table1, alltable);
    }
    //factor → NULL 
    if (production == "factor NULL") {
        return getType(ptree->children[0], table1, alltable);
    }
    // factor → LPAREN expr RPAREN
    if (production == "factor LPAREN expr RPAREN") {
        return getType(ptree->children[1], table1, alltable);
    }
    //factor → AMP lvalue
    if (production == "factor AMP lvalue") {
        check = getType(ptree->children[1], table1, alltable);
        if (check != "int") {
            throw string("ERROR : element before & must be int");
        }
        return "int*";
    }
    //factor → STAR factor
    if (production ==  "factor STAR factor" ) {
         check = getType(ptree->children[1], table1, alltable);
        if (check != "int*") {
            throw string("ERROR : element follow * must be int*");
        }
        return "int";
    }
    //factor → NEW INT LBRACK expr RBRACK
    if (production ==  "factor NEW INT LBRACK expr RBRACK" ) {
         check = getType(ptree->children[3], table1, alltable);
        if (check != "int") {
            throw string("ERROR : element after int must be int");
        }
        return  "int*";
    }
    //lvalue → ID
    if (production ==  "lvalue ID" ) {
        return getType(ptree->children[0], table1, alltable);
    } 
    //lvalue → STAR factor
    if (production ==  "lvalue STAR factor" ) {
         check = getType(ptree->children[1], table1, alltable);
        if (check != "int*") {
            throw string("ERROR : element follow * must be int*");
        }
        return "int";
    }
    //lvalue → LPAREN lvalue RPAREN 
    if (production ==  "lvalue LPAREN lvalue RPAREN" ) {
        return getType(ptree->children[1], table1, alltable);
    } 
    //arglist → expr
    if (production ==  "arglist expr" ) {
        return getType(ptree->children[0], table1, alltable);
    } 
    // arglist → expr COMMA arglist
    if (production ==  "arglist expr COMMA arglist" ) {
        return getType(ptree->children[0], table1, alltable);
    } 
    //factor → ID LPAREN RPAREN
    if (production == "factor ID LPAREN RPAREN") {
        string id = ptree->children[0]->tokens[1];
        if(alltable.find(id) == alltable.end()) {
            throw string("ERROR : procedure not decleared");
        }
        vector<string> arg = alltable[id].first;
        if(arg.size() != 0) {
            throw string("ERROR : missing argument");
        }
        return "int";
    }
    //factor → ID LPAREN arglist RPAREN
    if (production == "factor ID LPAREN arglist RPAREN") {
        string id = ptree->children[0]->tokens[1];
        if(alltable.find(id) == alltable.end()) {
            throw string("ERROR : procedure not decleared");
        }
        vector<string> arg = alltable[id].first;
        vector<string> arg1;
        struct tree *argtree = ptree->children[2];
        while(argtree->production != "arglist expr") {
            arg1.push_back(getType(argtree, table1, alltable));
            argtree = argtree->children[2];
        }
        arg1.push_back(getType(argtree, table1, alltable));
        if (arg == arg1) {
        } else {
           throw string("ERROR : incorrect arguments"); 
        }
        return "int";
    }
    return " ";
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
                string s1 = getType(ptree, table1, alltable);
                printSymtable(vtable1, "main", 2);
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
                int paran = 0;
                paran = count_para(ptree);
                vector<string> params;
                for(int i = 0; i < paran; i++) {
                    params.push_back(vtable1[i].second);
                }
                alltable.insert(make_pair(pname, make_pair(params,table1)));
                string s1 = getType(ptree, table1, alltable);
                printSymtable(vtable1,pname, paran);
            }
        }
    } catch(string msg) {
    cerr << msg << endl;
    }
    for(int i = 0; i < (int)alltr.size(); i++) {
        delete alltr[i];
    }
}