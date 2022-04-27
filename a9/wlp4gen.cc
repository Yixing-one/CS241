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
    string type;
    vector <string> tokens;
    vector <tree *> children;
    ~tree() {
        for(int i = 0; i  < (int) children.size() ; i++) {
            delete children[i];
        }
    }
} ;
struct symbolTable {
    vector<pair<string, string>> params;
    map<string, pair<string, int> > variables;
};

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

void buildptree (struct tree *ptree) {
    string inputline;
    getline(cin, inputline);
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
             buildptree(ptree1);
             ptree->children.push_back(ptree1);
        }
    }
}

string getType(struct tree * ptree, struct symbolTable *table1) {
    string production = ptree->production;
    string check;
    string check1;
    if(ptree->tokens[0] == "NUM") {
        ptree->type = "int";
        return "int";
    }
    if(ptree->tokens[0] == "ID") {
        if(table1->variables.find(ptree->tokens[1]) != table1->variables.end()) {
            ptree->type = table1->variables[ptree->tokens[1]].first;
            return ptree->type;
        } else {
            ptree->type = "int";
            return ptree->type;
        }
    }
    if(ptree->tokens[0] == "NULL") {
        ptree->type = "int*";
        return "int*";
    }
    if(ptree->tokens[0] == "factor ID") {
        ptree->type = getType(ptree->children[0], table1);
        return ptree->type;
    }
    //procedures → procedure procedures
    if(production == "procedures procedure procedures") {
        check = getType(ptree->children[0], table1);
        check = getType(ptree->children[1], table1);
    }
    //procedures → main
    if(production == "procedures main") {
        check = getType(ptree->children[0], table1);
    }
    //procedure → INT ID LPAREN params RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE 
    if (production == "procedure INT ID LPAREN params RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE") {
        check = getType(ptree->children[6], table1);
        check = getType(ptree->children[7], table1);
        check = getType(ptree->children[9], table1);
    }
    //main → INT WAIN LPAREN dcl COMMA dcl RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE 
    if (production == "main INT WAIN LPAREN dcl COMMA dcl RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE") {
        check = getType(ptree->children[8], table1);
        check = getType(ptree->children[9], table1);
        check = getType(ptree->children[11], table1);
    }
    //params →
    if(production == "params") {
    }
    //params → paramlist
    if(production == "params paramlist") {
        check = getType(ptree->children[0], table1);
    }
    //paramlist → dcl
    if(production == "paramlist dcl") {
        check = getType(ptree->children[0], table1);
    }
    //paramlist → dcl COMMA paramlist
    if(production == "paramlist dcl COMMA paramlist") {
        check = getType(ptree->children[0], table1);
        check = getType(ptree->children[2], table1);
    }
    //type → INT
    if(production == "type INT") {
        ptree->type = "int";
        return "int";
    }
    //type → INT STAR
    if(production == "type INT STAR") {
        ptree->type = "int*";
        return "int*";
    }
    // dcls ->
    if(production == "dcls") {
    }
    //dcls → dcls dcl BECOMES NUM SEMI
    if (production == "dcls dcls dcl BECOMES NUM SEMI") {
        check = getType(ptree->children[0], table1);
    }
    //dcls → dcls dcl BECOMES NULL SEMI
    if (production == "dcls dcls dcl BECOMES NULL SEMI") {
        check = getType(ptree->children[0], table1);
    }
    //dcl → type ID
    if(production ==  "dcl type ID") { 
         check = getType(ptree->children[0], table1);
         check1 = getType(ptree->children[1], table1);
         ptree->type = check;
         return check;
     }
     //statements →
     if(production == "statements") {
    }
    //statements → statements statement
    if(production == "statements statements statement") {
        check = getType(ptree->children[0], table1);
        check = getType(ptree->children[1], table1);
    }  
    //statement → lvalue BECOMES expr SEMI
    if (production == "statement lvalue BECOMES expr SEMI") {
       check = getType(ptree->children[0], table1);
       check1 = getType(ptree->children[2], table1);
    }
    //statement → IF LPAREN test RPAREN LBRACE statements RBRACE ELSE LBRACE statements RBRACE 
    if (production == "statement IF LPAREN test RPAREN LBRACE statements RBRACE ELSE LBRACE statements RBRACE") {
       check = getType(ptree->children[2], table1);
       check = getType(ptree->children[5], table1);
       check = getType(ptree->children[9], table1);
    }
    //statement → WHILE LPAREN test RPAREN LBRACE statements RBRACE 
    if (production == "statement WHILE LPAREN test RPAREN LBRACE statements RBRACE") {
       check = getType(ptree->children[2], table1);
       check = getType(ptree->children[5], table1);
    }
    //statement → PRINTLN LPAREN expr RPAREN SEMI
     if (production == "statement PRINTLN LPAREN expr RPAREN SEMI") {
       check = getType(ptree->children[2], table1);
    }
    //statement → DELETE LBRACK RBRACK expr SEMI
    if (production == "statement DELETE LBRACK RBRACK expr SEMI") {
        check = getType(ptree->children[3], table1);
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
        check = getType(ptree->children[0], table1);
        check1 = getType(ptree->children[2], table1);
        ptree->type = "int";
        return "int";
     }
     //expr → term
     if (production == "expr term") {
        ptree -> type = getType(ptree->children[0], table1);
        return ptree -> type;
     }
     //expr → expr PLUS term
     if(production == "expr expr PLUS term") {
        check = getType(ptree->children[0], table1);
        check1 = getType(ptree->children[2], table1);
        if((check == "int") && (check1 == "int")){
            ptree->type = "int";
            return "int";
        } else if ((check == "int*") && (check1 == "int")) {
            ptree->type = "int*";
            return "int*";
        } else if ((check == "int") && (check1 == "int*")) {
            ptree->type = "int*";
            return "int*";
        } else {
        }
    }
    //expr → expr MINUS term 
    if(production == "expr expr MINUS term") {
        check = getType(ptree->children[0], table1);
        check1 = getType(ptree->children[2], table1);
        if((check == "int") && (check1 == "int")){
            ptree->type = "int";
            return "int";
        } else if ((check == "int*") && (check1 == "int")) {
            ptree->type = "int*";
            return"int*";
        } else if ((check == "int*") && (check1 == "int*")) {
            ptree->type = "int";
            return "int";
        } else {
        }
    }
    //term → factor 
    if (production == "term factor") {
        ptree->type = getType(ptree->children[0], table1);
        return ptree->type;
    }
    //term → term STAR factor 
    //term → term SLASH factor 
    //term → term PCT factor
    if (production ==  "term term STAR factor" || production == "term term SLASH factor"
        || production == "term term PCT factor" ) {
            check = getType(ptree->children[0], table1);
            check1 = getType(ptree->children[2], table1);
            ptree->type = "int";
            return "int";
    }
    //factor → ID 
    if (production == "factor ID") {
        ptree->type = getType(ptree->children[0], table1);
        return ptree->type;
    }
    //factor → NUM  
    if (production == "factor NUM") {
        ptree->type = getType(ptree->children[0], table1);
        return ptree->type;
    }
    //factor → NULL 
    if (production == "factor NULL") {
        ptree->type = getType(ptree->children[0], table1);
        return ptree->type;
    }
    // factor → LPAREN expr RPAREN
    if (production == "factor LPAREN expr RPAREN") {
        ptree->type = getType(ptree->children[1], table1);
        return ptree->type;
    }
    //factor → AMP lvalue
    if (production == "factor AMP lvalue") {
        check = getType(ptree->children[1], table1);
        ptree->type = "int*";
        return "int*";
    }
    //factor → STAR factor
    if (production ==  "factor STAR factor" ) {
        check = getType(ptree->children[1], table1);
        ptree->type = "int";
        return "int";
    }
    //factor → NEW INT LBRACK expr RBRACK
    if (production ==  "factor NEW INT LBRACK expr RBRACK" ) {
         check = getType(ptree->children[3], table1);
         ptree->type = "int*";
        return  "int*";
    }
    //lvalue → ID
    if (production ==  "lvalue ID" ) {
        ptree->type = getType(ptree->children[0], table1);
        return ptree->type;
    } 
    //lvalue → STAR factor
    if (production ==  "lvalue STAR factor" ) {
        check = getType(ptree->children[1], table1);
        ptree->type = "int";
        return "int";
    }
    //lvalue → LPAREN lvalue RPAREN 
    if (production ==  "lvalue LPAREN lvalue RPAREN" ) {
        ptree->type = getType(ptree->children[1], table1);
        return ptree->type;
    } 
    //arglist → expr
    if (production ==  "arglist expr" ) {
        ptree->type = getType(ptree->children[0], table1);
        return ptree->type;
    } 
    // arglist → expr COMMA arglist
    if (production ==  "arglist expr COMMA arglist" ) {
        string s = getType(ptree->children[2], table1);
        ptree->type = getType(ptree->children[0], table1);
        return ptree->type;
    } 
    //factor → ID LPAREN RPAREN
    if (production == "factor ID LPAREN RPAREN") {
        ptree->type = "int";
        return "int";
    }
    //factor → ID LPAREN arglist RPAREN
    if (production == "factor ID LPAREN arglist RPAREN") {
        string s = getType(ptree->children[2], table1);
        ptree->type = "int";
        return "int";
    }
    return " ";
}

int symbol_location;

void get_name_type(struct tree *ptree, string &name, string &type) {
   name = ptree->children[1]->tokens[1];
    type = ptree->children[0]->production;
    if(type == "type INT") {
        type = "int";
    } else {
        type = "int*";
    }
}

void getSymtable(struct tree *ptree, struct symbolTable *symTable) {
    if(ptree->production == "dcl type ID") {
        string name, type;
        get_name_type(ptree, name, type);
        symbol_location -= 4;
        symTable->variables.insert(make_pair(name, make_pair(type, symbol_location)));
    }
    if(ptree->production == "procedure INT ID LPAREN params RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE") {
        symbol_location = 4;
        string name;
        string type;
        struct tree *pt = ptree->children[3];
        while(true) {
            if (pt->production == "params"){
                break;
            }
            if(pt->production == "params paramlist"){
                pt = pt->children[0];
            }
            if(pt->production == "paramlist dcl"){
                get_name_type(pt->children[0],name,type);
                symTable->params.push_back(make_pair(name,type));
                break;
            }
            if(pt->production == "paramlist dcl COMMA paramlist"){
                get_name_type(pt->children[0],name,type);
                symTable->params.push_back(make_pair(name,type));
                pt = pt->children[2];
            }
        } 
    }
    if(ptree->production == "main INT WAIN LPAREN dcl COMMA dcl RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE") {
        symbol_location = 4;
        string name;
        string type;
        get_name_type(ptree->children[3],name,type);
        symTable->params.push_back(make_pair(name,type));
        get_name_type(ptree->children[5],name,type);
        symTable->params.push_back(make_pair(name,type));
    }
    for(int i = 0; i < (int)ptree->children.size(); i++) {
        getSymtable(ptree->children[i], symTable);
    }
}

void GenerateMainCode(struct tree *ptree){
    cout << "; import useful function" << endl;
    //import
	cout << ".import print" << endl;
    cout << ".import init" << endl;
    cout << ".import new" << endl;
    cout << ".import delete" << endl;
    //store value
    cout << "lis $11" << endl;
	cout << ".word 1" << endl;
    cout << "lis $4" << endl;
	cout << ".word 4" << endl;
    cout << "sub $29, $30, $4" << endl;
    //push register with necessary values
    cout << "sw $1, -4($30)" << endl;
	cout << "sub $30, $30, $4" << endl;
	cout << "sw $2, -4($30)" << endl;
	cout << "sub $30, $30, $4" << endl;
    //set $2 to be 0 if the second argument is an int
    if(ptree->children[3]->children[0]->production == "type INT") {
        cout << "add $2, $0, $0" << endl;
    }
    //initial the function
    cout << "sw $31, -4($30)" << endl;
	cout << "sub $30, $30, $4" << endl;
    cout << "lis $5" << endl;
    cout << ".word init" << endl;
	cout << "jalr $5" << endl;
	cout << "lw $31, 0($30)" << endl;
	cout << "add $30, $30, $4" << endl;
    cout << "; End function set" << endl;
}

string functionName;
int ifvalue = -1;
int whilevalue = -1;
int deletevalue = -1;

void GenerateCode(struct tree *ptree, map<string, struct symbolTable *> alltable) {
    string production = ptree->production;
    if(ptree->tokens[0] == "NUM") {
    }
    if(ptree->tokens[0] == "ID") {
    }
    if(ptree->tokens[0] == "NULL") {
    }
    //procedures → main
    if(production == "procedures procedure procedures") {
        GenerateCode(ptree->children[1], alltable);
        GenerateCode(ptree->children[0], alltable);
    }
    //procedures → procedure procedures
    if(production == "procedures main") {
        GenerateCode(ptree->children[0], alltable);
    }
    //procedure → INT ID LPAREN params RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE 
    if (production == "procedure INT ID LPAREN params RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE") {
        functionName = ptree->children[1]->tokens[1];
        GenerateCode(ptree->children[3], alltable);
        cout << "F" << functionName << ":" << endl;
        //store input values
        cout << "sub $29, $30, $4" << endl;
        GenerateCode(ptree->children[6], alltable);
        cout << "sw $1, -4($30)" << endl;
		cout << "sub $30, $30, $4" << endl;
		cout << "sw $2, -4($30)" << endl;
		cout << "sub $30, $30, $4" << endl;
		cout << "sw $5, -4($30)" << endl;
		cout << "sub $30, $30, $4" << endl;
		cout << "sw $6, -4($30)" << endl;
		cout << "sub $30, $30, $4" << endl;
		cout << "sw $7, -4($30)" << endl;
		cout << "sub $30, $30, $4" << endl;
		cout << "sw $8, -4($30)" << endl;
		cout << "sub $30, $30, $4" << endl;
        //run statements
        GenerateCode(ptree->children[7], alltable);
        GenerateCode(ptree->children[9], alltable);
        //Pop input value back to registers
		cout << "lw $8, 0($30)" << endl;
		cout << "add $30, $30, $4" << endl;
		cout << "lw $7, 0($30)" << endl;
		cout<< "add $30, $30, $4" << endl;
		cout << "lw $6, 0($30)" << endl;
		cout << "add $30, $30, $4" << endl;
		cout << "lw $5, 0($30)" << endl;
		cout << "add $30, $30, $4" << endl;
		cout << "lw $2, 0($30)" << endl;
		cout << "add $30, $30, $4" << endl;
		cout << "lw $1, 0($30)" << endl;
		cout << "add $30, $30, $4" << endl;
		//return back to original function
		cout << "add $30, $29, $4" << endl;
		cout << "jr $31" << endl;
    }
    //main → INT WAIN LPAREN dcl COMMA dcl RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE 
    if (production == "main INT WAIN LPAREN dcl COMMA dcl RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE") {
        GenerateMainCode(ptree);
        functionName = "wain";
        GenerateCode(ptree->children[8], alltable);
        GenerateCode(ptree->children[9], alltable);
        GenerateCode(ptree->children[11], alltable);
        cout << "add $30, $29, $4" << endl;
	    cout << "jr $31" << endl;
    }
    //params →
    if(production == "params") {
    }
    //params → paramlist
    if(production == "params paramlist") {
        int update_offset = alltable[functionName]->params.size() * 4;
        for (map<string, pair<string, int> > :: iterator it =  alltable[functionName]->variables.begin(); it != alltable[functionName]->variables.end(); it++) {
            it->second.second += update_offset;
        }
    }
    //paramlist → dcl
    if(production == "paramlist dcl") {
    }
    //paramlist → dcl COMMA paramlist
    if(production == "paramlist dcl COMMA paramlist") {
    }
    //type → INT
    if(production == "type INT") {   
    }
    //type → INT STAR
    if(production == "type INT STAR") {
    }
    // dcls ->
    if(production == "dcls") {
    }
    //dcls → dcls dcl BECOMES NUM SEMI
    if (production == "dcls dcls dcl BECOMES NUM SEMI") {
        GenerateCode(ptree->children[0], alltable);
        //find the value of the variable to be stored at $3
        string num = ptree->children[3]->tokens[1];
        //find variable name
        string varaibleName = ptree->children[1]->children[1]->tokens[1];
        int offset = alltable[functionName]->variables[varaibleName].second;
        //generate code
        cout << "lis $3" << endl;
        cout << ".word " << num << endl;
        cout << "sw $3, " << to_string(offset) << "($29)" << endl; 
        cout << "sub $30, $30, $4" << endl;  
    }
    //dcls → dcls dcl BECOMES NULL SEMI
    if (production == "dcls dcls dcl BECOMES NULL SEMI") {
        GenerateCode(ptree->children[0], alltable);
        cout << "add $3, $0, $11" << endl;
        //find variable name
		string varaibleName = ptree->children[1]->children[1]->tokens[1];
		int offset = alltable[functionName]->variables[varaibleName].second;
		cout << "sw $3, " << to_string(offset) << "($29)" << endl; 
        cout << "sub $30, $30, $4" << endl; 

    }
    //dcl → type ID
    if(production ==  "dcl type ID") { 
     }
     //statements →
    if(production == "statements") {
    }
    //statements → statements statement
    if(production == "statements statements statement") {
        GenerateCode(ptree->children[0], alltable);
        GenerateCode(ptree->children[1], alltable);
    }  
    //statement → lvalue BECOMES expr SEMI
    if (production == "statement lvalue BECOMES expr SEMI") {
        GenerateCode(ptree->children[2], alltable);
        struct tree *temp = ptree->children[0];
        if(temp->production == "lvalue LPAREN lvalue RPAREN") {
            while (temp->production == "lvalue LPAREN lvalue RPAREN"){
                temp = temp->children[1];
			}
        } 
        if(temp->production == "lvalue ID"){
			string varaibleName = temp->children[0]->tokens[1];
			int offset = alltable[functionName]->variables[varaibleName].second;
			cout << "sw $3, " << to_string(offset) << "($29)" << endl;	
		}
        if(temp->production == "lvalue STAR factor"){
            //push $3
            cout << "sw $3, -4($30)" << endl;
	        cout << "sub $30, $30, $4" << endl;
            GenerateCode(temp->children[1], alltable);
            //pop $5
            cout << "lw $5, 0($30)" << endl;
	        cout << "add $30, $30, $4" << endl;
            cout << "sw $5, 0($3)" << endl;
		}
    }
    //statement → IF LPAREN test RPAREN LBRACE statements RBRACE ELSE LBRACE statements RBRACE 
    if (production == "statement IF LPAREN test RPAREN LBRACE statements RBRACE ELSE LBRACE statements RBRACE") { 
        ifvalue += 1;
        int ifvalue2 = ifvalue;
        //generate code for condition
        GenerateCode(ptree->children[2], alltable);
        //if condition fail
        cout << "bne $3, $11, else" << to_string(ifvalue2) << endl;
        //if condition pass
        GenerateCode(ptree->children[5], alltable);
        cout << "beq $0, $0, endif" << to_string(ifvalue2) << endl;
		cout << "else" << to_string(ifvalue2) << ":" << endl;
		GenerateCode(ptree->children[9], alltable);
		cout << "endif" << to_string(ifvalue2) << ":" << endl;
    }

    //statement → WHILE LPAREN test RPAREN LBRACE statements RBRACE 
    if (production == "statement WHILE LPAREN test RPAREN LBRACE statements RBRACE") {
        whilevalue +=1;
        int whilevalue2 = whilevalue;
		cout << "whileloop" << to_string(whilevalue2) << ":" << endl;
        //generate code for condition
        GenerateCode(ptree->children[2], alltable);
        //if condition fail
        cout << "beq $3, $0, done" << to_string(whilevalue2) << endl;
		//if condition pass
		GenerateCode(ptree->children[5], alltable);
		//go back to the begining of the loop
		cout << "beq $0, $0, whileloop" << to_string(whilevalue2) << endl;
		cout << "done" << to_string(whilevalue2) << ":" << endl;
		return;
    }
    //statement → PRINTLN LPAREN expr RPAREN SEMI
     if (production == "statement PRINTLN LPAREN expr RPAREN SEMI") {
		GenerateCode(ptree->children[2], alltable);
		cout << "add $1, $3, $0" << endl;
        //push $31
        cout << "sw $31, -4($30)" << endl;
	    cout << "sub $30, $30, $4" << endl;
        //call print
        cout << "lis $10" << endl;
	    cout << ".word print" << endl;
        cout << "jalr $10" << endl;
        //pop $31
        cout << "lw $31, 0($30)" << endl;
        cout << "add $30, $30, $4" << endl;
    }
    //statement → DELETE LBRACK RBRACK expr SEMI
    if (production == "statement DELETE LBRACK RBRACK expr SEMI") {
        GenerateCode(ptree->children[3], alltable);
        deletevalue += 1;
        cout << "beq $3, $11, goto" << to_string(deletevalue) << endl;
		cout << "add $1, $3, $0" << endl;
        //dealloc memory
        cout << "sw $31, -4($30)" << endl;
	    cout << "sub $30, $30, $4" << endl;
        cout << "lis $10" << endl;
	    cout << ".word delete" << endl;
	    cout << "jalr $10" << endl;
        cout << "lw $31, 0($30)" << endl;
        cout << "add $30, $30, $4" << endl;
        cout <<  "goto" << to_string(deletevalue) << ":" << endl;

    }
    //test → expr EQ expr  
    //test → expr NE expr  
    //test → expr LT expr 
    //test → expr LE expr  
    //test → expr GE expr  
    //test → expr GT expr 
     if (production == "test expr EQ expr") {
		GenerateCode(ptree->children[0], alltable);
		cout << "add $6, $3, $0" << endl;
		GenerateCode(ptree->children[2], alltable);
        if (ptree->children[0]->type == "int*"){
			cout << "sltu $7, $6, $3" << endl;
			cout << "sltu $8, $3, $6" << endl;
		}
		if (ptree->children[0]->type == "int"){
			cout << "slt $7, $6, $3" << endl;
			cout << "slt $8, $3, $6" << endl;
		}
		cout << "add $3, $7, $8" << endl;
		cout << "sub $3, $11, $3" << endl;
     } 
      if (production == "test expr NE expr") {
		GenerateCode(ptree->children[0], alltable);
		cout << "add $6, $3, $0" << endl;
		GenerateCode(ptree->children[2], alltable);
        if (ptree->children[0]->type == "int*"){
			cout << "sltu $7, $6, $3" << endl;
			cout << "sltu $8, $3, $6" << endl;
		}
		if (ptree->children[0]->type == "int"){
			cout << "slt $7, $6, $3" << endl;
			cout << "slt $8, $3, $6" << endl;
		}
		cout << "add $3, $7, $8" << endl;
     } 
     if (production == "test expr LT expr") {
		GenerateCode(ptree->children[0], alltable);
		cout << "add $6, $3, $0" << endl;
		GenerateCode(ptree->children[2], alltable);
        if (ptree->children[0]->type == "int*"){
			cout << "sltu $3, $6, $3" << endl;
		}
		if (ptree->children[0]->type == "int"){
			cout << "slt $3, $6, $3" << endl;
		}
     }
     if (production == "test expr GT expr") {
		GenerateCode(ptree->children[0], alltable);
		cout << "add $6, $3, $0" << endl;
		GenerateCode(ptree->children[2], alltable);
        if (ptree->children[0]->type == "int*"){
			cout << "sltu $3, $3, $6" << endl;
		}
		if (ptree->children[0]->type == "int"){
			cout << "slt $3, $3, $6" << endl;
		}
     }
     if (production == "test expr LE expr") {
		GenerateCode(ptree->children[0], alltable);
		cout << "add $6, $3, $0" << endl;
		GenerateCode(ptree->children[2], alltable);
        if (ptree->children[0]->type == "int*"){
			cout << "sltu $3, $6, $3" << endl;
		}
		if (ptree->children[0]->type == "int"){
			cout << "slt $3, $6, $3" << endl;
		}
     }
     if (production == "test expr LE expr") {
		GenerateCode(ptree->children[0], alltable);
		cout << "add $6, $3, $0" << endl;
		GenerateCode(ptree->children[2], alltable);
        if (ptree->children[0]->type == "int*"){
			cout << "sltu $3, $3, $6" << endl;
		}
		if (ptree->children[0]->type == "int"){
			cout << "slt $3, $3, $6" << endl;
		}
		cout << "sub $3, $11, $3" << endl;
     } 
      if (production == "test expr GE expr") {
		GenerateCode(ptree->children[0], alltable);
		cout << "add $6, $3, $0" << endl;
		GenerateCode(ptree->children[2], alltable);
        if (ptree->children[0]->type == "int*"){
			cout << "sltu $3, $6, $3" << endl;
		}
		if (ptree->children[0]->type == "int"){
			cout << "slt $3, $6, $3" << endl;
		}
		cout << "sub $3, $11, $3" << endl;
     } 
     //expr → term
     if (production == "expr term") {
          GenerateCode(ptree->children[0], alltable);
     }
     //expr → expr PLUS term
     if(production == "expr expr PLUS term") {
        GenerateCode(ptree->children[0], alltable);
        //push register $3
        cout << "sw $3, -4($30)" << endl;
	    cout << "sub $30, $30, $4" << endl;
        GenerateCode(ptree->children[2], alltable);
        //pop $3 to $5
        cout << "lw $5, 0($30)" << endl;
	    cout << "add $30, $30, $4" << endl;
        //perform addition: int = int + int 
        if((ptree->children[0]->type == "int") && (ptree->children[2]->type == "int")) {
            cout << "add $3, $5, $3" << endl;
        }
        //perform addition: int* = int + int* 
        if((ptree->children[0]->type == "int") && (ptree->children[2]->type == "int*")) {
            cout << "mult $5, $4" << endl;
			cout << "mflo $5" << endl;
            cout << "add $3, $5, $3" << endl;
        }
        //perform addition: int* = int* + int 
        if((ptree->children[0]->type == "int*") && (ptree->children[2]->type == "int")) {
            cout << "mult $3, $4" << endl;
            cout << "mflo $3" << endl;
            cout << "add $3, $5, $3" << endl;
        }
    }
    //expr → expr MINUS term 
    if(production == "expr expr MINUS term") {
        GenerateCode(ptree->children[0], alltable);
        //push register $3
        cout << "sw $3, -4($30)" << endl;
	    cout << "sub $30, $30, $4" << endl;
        GenerateCode(ptree->children[2], alltable);
        //pop $3 to $5
        cout << "lw $5, 0($30)" << endl;
	    cout << "add $30, $30, $4" << endl;
        //perform addition: int = int - int 
        if((ptree->children[0]->type == "int") && (ptree->children[2]->type == "int")){
            cout << "sub $3, $5, $3" << endl;
        }
        //perform addition: int* = int* - int 
        if((ptree->children[0]->type == "int*") && (ptree->children[2]->type == "int")) {
            cout << "mult $3, $4" << endl;
			cout << "mflo $3" << endl;
            cout << "sub $3, $5, $3" << endl;
        }
        //perform addition: int = int* - int* 
        if((ptree->children[0]->type == "int*") && (ptree->children[2]->type == "int*")) {
            cout << "sub $3, $5, $3" << endl;
			cout << "div $3, $4" << endl;
			cout << "mflo $3" << endl;
        }
    }
    //term → factor 
    if (production == "term factor") {
        GenerateCode(ptree->children[0], alltable);
    }
    //term → term STAR factor 
    //term → term SLASH factor 
    //term → term PCT factor
    if (production ==  "term term STAR factor") {
        GenerateCode(ptree->children[0], alltable);
        cout << "sw $3, -4($30)" << endl;
	    cout << "sub $30, $30, $4" << endl;
        GenerateCode(ptree->children[2], alltable);
        cout << "lw $5, 0($30)" << endl;
	    cout << "add $30, $30, $4" << endl;
        cout << "mult $5, $3" << endl;
        cout << "mflo $3" << endl;
    }
    if (production == "term term SLASH factor") {
        GenerateCode(ptree->children[0], alltable);
        cout << "sw $3, -4($30)" << endl;
	    cout << "sub $30, $30, $4" << endl;
        GenerateCode(ptree->children[2], alltable);
        cout << "lw $5, 0($30)" << endl;
	    cout << "add $30, $30, $4" << endl;
        cout << "div $5, $3" << endl;
        cout << "mflo $3" << endl;
    }
    if (production == "term term PCT factor" ) {
        GenerateCode(ptree->children[0], alltable);
        cout << "sw $3, -4($30)" << endl;
	    cout << "sub $30, $30, $4" << endl;
        GenerateCode(ptree->children[2], alltable);
        cout << "lw $5, 0($30)" << endl;
	    cout << "add $30, $30, $4" << endl;
        cout << "div $5, $3" << endl;
        cout << "mfhi $3" << endl;
    }
    //factor → ID 
    if (production == "factor ID") {
        //find store memory location for $3
        string varaibleName = ptree->children[0]->tokens[1];
        int offset = alltable[functionName]->variables[varaibleName].second;
        cout << "lw $3, " << to_string(offset) << "($29)" << endl;
    }
    //factor → NUM  
    if (production == "factor NUM") {
        string num = ptree->children[0]->tokens[1];
        cout << "lis $3" << endl;
		cout << ".word " << num << endl;
    }
    //factor → NULL 
    if (production == "factor NULL") {
        cout << "add $3, $0, $11" << endl;
    }
    // factor → LPAREN expr RPAREN
    if (production == "factor LPAREN expr RPAREN") {
        GenerateCode(ptree->children[1], alltable);
    }
    //factor → AMP lvalue
    if (production == "factor AMP lvalue") {
        string rule = ptree->children[1]->production;
        tree * temp = ptree->children[1];
        if(temp->production == "lvalue LPAREN lvalue RPAREN") {
            while(temp->production== "lvalue LPAREN lvalue RPAREN") {
                temp = temp->children[1];
            }
        }
        if(temp->production == "lvalue ID") {
            string varaibleName = temp->children[0]->tokens[1];
            int offset = alltable[functionName]->variables[varaibleName].second;
            cout <<"lis $3" << endl;
            cout << ".word " << offset << endl;
            cout << "add $3, $3, $29" << endl;
        }
        if(temp->production == "lvalue STAR factor") {
            GenerateCode(temp->children[1], alltable);
        }
    }
    //factor → STAR factor
    if (production ==  "factor STAR factor" ) {
        GenerateCode(ptree->children[1], alltable);
        cout << "lw $3, 0($3)" << endl;
    }
    //factor → NEW INT LBRACK expr RBRACK
    if (production ==  "factor NEW INT LBRACK expr RBRACK" ) {
        GenerateCode(ptree->children[3], alltable);
        cout << "add $1, $3, $0" << endl;
        //allocate new element
        cout << "sw $31, -4($30)" << endl;
	    cout << "sub $30, $30, $4" << endl;
        cout << "lis $10" << endl;
	    cout << ".word new" << endl;
	    cout << "jalr $10" << endl;
        cout << "lw $31, 0($30)" << endl;
	    cout << "add $30, $30, $4" << endl;
        //check if the allocation is successful 
        cout << "bne $3, $0, 1" << endl;
		cout << "add $3, $11, $0" << endl;
    }
    //lvalue → LPAREN lvalue RPAREN 
    if (production ==  "lvalue LPAREN lvalue RPAREN" ) {
        GenerateCode(ptree->children[1], alltable);
    } 
    //arglist → expr
    if (production ==  "arglist expr" ) {
        GenerateCode(ptree->children[0], alltable);
        cout << "sw $3, -4($30)" << endl;
	    cout << "sub $30, $30, $4" << endl;
    } 
    // arglist → expr COMMA arglist
    if (production ==  "arglist expr COMMA arglist" ) {
        GenerateCode(ptree->children[0], alltable);
        cout << "sw $3, -4($30)" << endl;
	    cout << "sub $30, $30, $4" << endl;
        GenerateCode(ptree->children[2], alltable);
    } 
    //factor → ID LPAREN RPAREN
    if (production == "factor ID LPAREN RPAREN") {
        cout << "sw $29, -4($30)" << endl;
		cout << "sub $30, $30, $4" << endl;
        cout << "sw $31, -4($30)" << endl;
	    cout << "sub $30, $30, $4" << endl;
        //call the fucntion
        string function = ptree->children[0]->tokens[1];
        cout << "lis $5" << endl;
		cout << ".word " << "F" << function << endl;
		cout << "jalr $5" << endl;
        cout << "lw $31, 0($30)" << endl;
	    cout << "add $30, $30, $4" << endl;
        cout << "lw $29, 0($30)" << endl;
		cout << "add $30, $30, $4" << endl;
    }
    //factor → ID LPAREN arglist RPAREN
    if (production == "factor ID LPAREN arglist RPAREN") {
        //Push $29 and $31
		cout << "sw $29, -4($30)" << endl;
		cout << "sub $30, $30, $4" << endl;
        cout << "sw $31, -4($30)" << endl;
	    cout << "sub $30, $30, $4" << endl;
        GenerateCode(ptree->children[2], alltable);
        string function =  ptree->children[0]->tokens[1];
        //Call the procedure
		cout << "lis $5" << endl;
		cout << ".word " << "F" << function << endl;
		cout << "jalr $5" << endl;
        //get memory for the new function 
        int parasize = (alltable[function]->params.size()) *4;
        cout << "lis $5" << endl;
        cout << ".word " << to_string(parasize) << endl;
        cout << "add $30, $30, $5" << endl;
        //pop $31 and $29
        cout << "lw $31, 0($30)" << endl;
	    cout << "add $30, $30, $4" << endl;
        cout << "lw $29, 0($30)" << endl;
		cout << "add $30, $30, $4" << endl;
    }
}

int main () {
    vector<tree*> alltr;
    map<string, struct symbolTable *>alltable;
    try {
        string inputline;
        getline(cin, inputline); // start BOF procedures EOF
        getline(cin, inputline); // BOF BOF
        while (getline(cin, inputline)) {
            if(inputline == "EOF EOF"){
                break;
            }
            struct tree *ptree = new struct tree();
            buildptree (ptree);
            alltr.push_back(ptree);
            struct symbolTable *symTable = new struct symbolTable();
            getSymtable(ptree, symTable);
            string pname = ptree->children[1]->tokens[1];
            alltable.insert(make_pair(pname, symTable));
            string s = getType(ptree, symTable);
        }
         //comment code
        for(int i = (alltr.size()-1); i > -1; i --) {
            GenerateCode(alltr[i], alltable);
        }
        for(int i = 0; i < (int)alltr.size(); i++) {
            delete alltr[i];
        }
        for (auto it = alltable.begin(); it != alltable.end();) {
            delete it->second;
            it = alltable.erase(it);  // update here
        }
    } catch(string msg) {
    cerr << msg << endl;
    }
}