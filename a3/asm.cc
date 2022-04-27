#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <map>
#include <string>
#include <math.h> 
#include "scanner.h"
using namespace std;

void printWORD(int i) {
  char c1 = i >> 24;
   cout << c1;
  c1 = i >> 16;
   cout << c1;
  c1 = i >> 8;
   cout << c1; 
  c1 = i;
   cout << c1;
}

void pop_front(std::vector<Token> &vecLine) { 
  if(vecLine.size() > 0) {
    vecLine.erase(vecLine.begin());
  }
}

/*
 * C++ Starter code for CS241 A3
 * All code requires C++14, so if you're getting compile errors make sure to
 * use -std=c++14.
 *
 * This file contains the main function of your program. By default, it just
 * prints the scanned list of tokens back to standard output.
 */ 
int main() {
  std::string line;
  int memoryLocation = 0;
  map<string, int> symbolTable;
  vector<vector<Token>> inputLine; 

  try {
    while (getline(std::cin, line)) {
      // This example code just prints the scanned tokens on each line.
      std::vector<Token> tokenLine = scan(line);
      inputLine.push_back(tokenLine);
    }
    //insert all the lable into symbol table
    for(int i = 0; i < (int)inputLine.size(); i++) {
      for (int j = 0; j < (int)inputLine[i].size(); j++ ) {
        if (inputLine[i][j].getKind() == Token::Kind::LABEL) {
            if ((j > 0)&&(inputLine[i][j-1].getKind() != Token::Kind::LABEL)) {
                cerr << "ERROR: there should not be instruction before label";
                return 1;
             } 
          string lexeme =inputLine[i][j].getLexeme();
          int lenlexeme = lexeme.length();
          string strLable = lexeme.substr(0,lenlexeme-1);
          if (symbolTable.count(strLable) == 0) {
              symbolTable.insert(pair<string, int>(strLable, memoryLocation));
              cerr << strLable << " " << memoryLocation << endl;
          } else {
              cerr <<"ERROR: invalid label, label already exists" << std::endl;
              return 1;
          }
        } else if (inputLine[i][j].getKind() == Token::Kind::WORD) {
            memoryLocation += 4;
            j++;
        } else if (inputLine[i][j].getKind() == Token::Kind::ID) {
          j = (int)inputLine[i].size();
          memoryLocation += 4;
        }
      }
    }

// symbolTable.insert(pair<string, int>(tokenLine[i + 1].getLexeme(), memoryLocation));
      // This code is just an example - you don't have to use a range-based
      // for loop in your actual assembler. You might find it easier to
      // use an index-based loop like this:
      // for(int i=0; i<tokenLine.size(); i++) { ... }
      int memory2 = 0;
     for(int i = 0; i < (int)inputLine.size(); i++) {
      for (int j = 0; j < (int)inputLine[i].size(); j++) {
        //check if the instruction is "WORD" 
        if (inputLine[i][j].getKind() == Token::Kind::WORD) {
          if (j + 1 < (int) inputLine[i].size()) {
            if (inputLine[i][j + 1].getKind() == Token::INT || inputLine[i][j + 1].getKind() == Token::HEXINT){
                if ((inputLine[i][j + 1].getKind() == Token::INT)&&(inputLine[i][j + 1].toNumber() >= (-1*pow(2,31))&&(inputLine[i][j + 1].toNumber() <= (pow(2,31)-1)))) {
                    printWORD (inputLine[i][j + 1].toNumber());
                } else if ((inputLine[i][j + 1].getKind() == Token::HEXINT)&&(inputLine[i][j + 1].toNumber() <= 0xffffffff)&&(inputLine[i][j + 1].toNumber() >= 0)){
                    printWORD (inputLine[i][j + 1].toNumber());
                }else {
                  cerr << (-1*pow(2,31));
                    cerr <<"ERROR: invalid input number, outofbound"<< std::endl;
                    return 1;
                }
            } else if (symbolTable.count(inputLine[i][j + 1].getLexeme()) == 1) {
                string value;
                printWORD ((int)symbolTable.find(inputLine[i][j + 1].getLexeme())->second);
            } else {
                cerr <<"ERROR: invalid input" <<endl;;
            }
          }
          j++;
          memory2 += 4;
        } else if (inputLine[i][j].getKind() == Token::Kind::ID) {
            if((inputLine[i][j].getLexeme() == "jr")&& (j==0)){
              if((inputLine[i].size() > 2) && (inputLine[i][2].getKind() != Token::Kind:: COMMENT)&&(inputLine[i][2].getKind() != Token::Kind:: WHITESPACE)) {
                cerr << "ERROR: there should not be value other than comment after jr instruction"<<endl;
                return 1;
              }
              if((j+1<(int)inputLine[i].size())&&(inputLine[i][j+1].getKind() == Token::Kind::REG)) {
                int mips = inputLine[i][j+1].toNumber();
                if((mips > 31) || (mips < 0)) {
                  cerr << "ERROR: the register value should be between 0 and 31"<<endl;
                  return 1;
                } else {
                  int mips2 = 0 << 26 | mips << 21 |8 << 0;
                  printWORD(mips2);
                }
              }else{
                cerr << "ERROR: a register input should follow ja" <<endl;
                return 1;
              }
              j++;
            }

            if((inputLine[i][j].getLexeme() == "jalr")&& (j==0)){
              if((inputLine[i].size() > 2) && (inputLine[i][2].getKind() != Token::Kind:: COMMENT)&&(inputLine[i][2].getKind() != Token::Kind:: WHITESPACE)) {
                cerr << "ERROR: there should not be value other than comment after jalr instruction"<<endl;
                return 1;
              }
              if((j+1<(int)inputLine[i].size())&&(inputLine[i][j+1].getKind() == Token::Kind::REG)) {
                int mips =  inputLine[i][j+1].toNumber();
                if((mips > 31) || (mips < 0)) {
                  cerr << "ERROR: the register value should be between 0 and 31"<<endl;
                  return 1;
                } else {
                  int mips2 = 0 << 26 | mips << 21 | 9;
                  printWORD(mips2);
                }
              }else{
                cerr << "ERROR: a register input should follow ja" <<endl;
                return 1;
              }
              j++;
            }

            if((inputLine[i][j].getLexeme() == "add")&& (j==0)){
              if((inputLine[i].size() > 6) && (inputLine[i][6].getKind() != Token::Kind:: COMMENT)&&(inputLine[i][6].getKind() != Token::Kind:: WHITESPACE)) {
                cerr << "ERROR: there should not be value other than comment after add instruction"<<endl;
                return 1;
              }
              if((j+5<(int)inputLine[i].size())&&(inputLine[i][j+1].getKind() == Token::Kind::REG)
              &&(inputLine[i][j+3].getKind() == Token::Kind::REG)&&(inputLine[i][j+5].getKind() == Token::Kind::REG)
              &&(inputLine[i][j+2].getKind() == Token::Kind::COMMA)&&(inputLine[i][j+4].getKind() == Token::Kind::COMMA)) {
                int mips = inputLine[i][j+1].toNumber();
                int mips1 = inputLine[i][j+3].toNumber();
                int mips2 = inputLine[i][j+5].toNumber();
                if((mips > 31) || (mips < 0) || (mips1 > 31) || (mips1 < 0)||(mips2 > 31) || (mips2 < 0)) {
                  cerr << "ERROR: the register value should be between 0 and 31"<<endl;
                  return 1;
                } else {
                  int mips3 = 0 << 26 | mips1 << 21 | mips2 << 16 | mips << 11 |32 << 0;
                  printWORD(mips3);
                }
              }else{
                cerr << "ERROR: three register input should follow add" <<endl;
                return 1;
              }
              j+=5;
            }

            if((inputLine[i][j].getLexeme() == "sub")&& (j==0)){
            if((inputLine[i].size() > 6) && (inputLine[i][6].getKind() != Token::Kind:: COMMENT)&&(inputLine[i][6].getKind() != Token::Kind:: WHITESPACE)) {
                cerr << "ERROR: there should not be value other than comment after sub instruction"<<endl;
                return 1;
              }
              if((j+5<(int)inputLine[i].size())&&(inputLine[i][j+1].getKind() == Token::Kind::REG)
              &&(inputLine[i][j+3].getKind() == Token::Kind::REG)&&(inputLine[i][j+5].getKind() == Token::Kind::REG)
              &&(inputLine[i][j+2].getKind() == Token::Kind::COMMA)&&(inputLine[i][j+4].getKind() == Token::Kind::COMMA)) {
                int mips = inputLine[i][j+1].toNumber();
                int mips1 = inputLine[i][j+3].toNumber();
                int mips2 = inputLine[i][j+5].toNumber();
                if((mips > 31) || (mips < 0) || (mips1 > 31) || (mips1 < 0)||(mips2 > 31) || (mips2 < 0)) {
                  cerr << "ERROR: the register value should be between 0 and 31"<<endl;
                  return 1;
                } else {
                  int mips3 = 0 << 26 | mips1 << 21 | mips2 << 16 | mips << 11 |34 << 0;
                  printWORD(mips3);
                }
              }else{
                cerr << "ERROR: three register input should follow sub" <<endl;
                return 1;
              }
              j+=5;
            }

            if((inputLine[i][j].getLexeme() == "slt")&& (j==0)){
              if((inputLine[i].size() > 6) && (inputLine[i][6].getKind() != Token::Kind:: COMMENT)&&(inputLine[i][6].getKind() != Token::Kind:: WHITESPACE)) {
                cerr << "ERROR: there should not be value other than comment after slt instruction"<<endl;
                return 1;
              }
              if((j+5<(int)inputLine[i].size())&&(inputLine[i][j+1].getKind() == Token::Kind::REG)
              &&(inputLine[i][j+3].getKind() == Token::Kind::REG)&&(inputLine[i][j+5].getKind() == Token::Kind::REG)
              &&(inputLine[i][j+2].getKind() == Token::Kind::COMMA)&&(inputLine[i][j+4].getKind() == Token::Kind::COMMA)) {
                int mips = inputLine[i][j+1].toNumber();
                int mips1 = inputLine[i][j+3].toNumber();
                int mips2 = inputLine[i][j+5].toNumber();
                if((mips > 31) || (mips < 0) || (mips1 > 31) || (mips1 < 0)||(mips2 > 31) || (mips2 < 0)) {
                  cerr << "ERROR: the register value should be between 0 and 31"<<endl;
                  return 1;
                } else {
                  int mips3 = 0 << 26 | mips1 << 21 | mips2 << 16 | mips << 11 |42 << 0;
                  printWORD(mips3);
                }
              }else{
                cerr << "ERROR: three register input add two comma should follow slt " <<endl;
                return 1;
              }
              j+=5;
            }

            if((inputLine[i][j].getLexeme() == "sltu")&& (j==0)){
              if((inputLine[i].size() > 6) && (inputLine[i][6].getKind() != Token::Kind:: COMMENT)&&(inputLine[i][6].getKind() != Token::Kind:: WHITESPACE)) {
                cerr << "ERROR: there should not be value other than comment after sltu instruction"<<endl;
                return 1;
              }
              if((j+5<(int)inputLine[i].size())&&(inputLine[i][j+1].getKind() == Token::Kind::REG)
              &&(inputLine[i][j+3].getKind() == Token::Kind::REG)&&(inputLine[i][j+5].getKind() == Token::Kind::REG)
              &&(inputLine[i][j+2].getKind() == Token::Kind::COMMA)&&(inputLine[i][j+4].getKind() == Token::Kind::COMMA)) {
                int mips = inputLine[i][j+1].toNumber();
                int mips1 = inputLine[i][j+3].toNumber();
                int mips2 = inputLine[i][j+5].toNumber();
                if((mips > 31) || (mips < 0) || (mips1 > 31) || (mips1 < 0)||(mips2 > 31) || (mips2 < 0)) {
                  cerr << "ERROR: the register value should be between 0 and 31"<<endl;
                  return 1;
                } else {
                  int mips3 = 0 << 26 | mips1 << 21 | mips2 << 16 | mips << 11 |43 << 0;
                  printWORD(mips3);
                }
              }else{
                cerr << "ERROR: three register input and two comma should follow sltu" <<endl;
                return 1;
              }
              j+=5;
            }

            if((inputLine[i][j].getLexeme() == "beq")&& (j==0)){
              if((inputLine[i].size() > 6) && (inputLine[i][6].getKind() != Token::Kind:: COMMENT)&&(inputLine[i][6].getKind() != Token::Kind:: WHITESPACE)) {
                cerr << "ERROR: there should not be value other than comment after beq instruction"<<endl;
                return 1;
              }
              if((j+5<(int)inputLine[i].size())&&(inputLine[i][j+1].getKind() == Token::Kind::REG)
              &&(inputLine[i][j+3].getKind() == Token::Kind::REG)&&((inputLine[i][j+5].getKind() == Token::Kind::HEXINT) || 
              (inputLine[i][j+5].getKind() == Token::Kind::INT))&&(inputLine[i][j+2].getKind() == Token::Kind::COMMA)
              &&(inputLine[i][j+4].getKind() == Token::Kind::COMMA)) {
                int mips = inputLine[i][j+1].toNumber();
                int mips1 = inputLine[i][j+3].toNumber();
                int mips2 = inputLine[i][j+5].toNumber();
                int64_t mips5 = 65535;
                int mips4 = mips2&mips5;
                if((mips > 31) || (mips < 0) || (mips1 > 31) || (mips1 < 0)||
                ((inputLine[i][j+5].getKind() == Token::Kind::INT)&&(mips2 > 32767)) 
                ||((inputLine[i][j+5].getKind() == Token::Kind::INT)&&(mips2 <  -32768))
                ||((inputLine[i][j+5].getKind() == Token::Kind::HEXINT)&&(mips2 <  0))
                ||((inputLine[i][j+5].getKind() == Token::Kind::HEXINT)&&(mips2 > 0xffff))){
                  cerr << "ERROR: the register value should be between 0 and 31 and the int must be in the range"<<endl;
                  return 1;
                } else {
                  int mips3 = 4 << 26 | mips << 21 | mips1 << 16 | mips4;
                  printWORD(mips3);
                }
              } else if ((j+5<(int)inputLine[i].size())&&(inputLine[i][j+1].getKind() == Token::Kind::REG)
              &&(inputLine[i][j+3].getKind() == Token::Kind::REG)
              &&(inputLine[i][j+2].getKind() == Token::Kind::COMMA)&&(inputLine[i][j+4].getKind() == Token::Kind::COMMA)) {
                if(symbolTable.count(inputLine[i][j + 5].getLexeme()) == 1) {
                    int mips = inputLine[i][j+1].toNumber();
                    int mips1 = inputLine[i][j+3].toNumber();
                    int mips2 = (int)symbolTable.find(inputLine[i][j + 5].getLexeme())->second;
                    int mips4 = (mips2-memory2-4)/4;
                    int64_t mips5 = 65535;
                    int mips6 = mips4&mips5;
                    int mips3 = 4 << 26 | mips << 21 | mips1 << 16 | mips6;
                    printWORD(mips3);
                } else {
                  cerr << "ERROR: two registers input, one int/label and two comma should follow beq, label not valid" <<endl;
                  return 1;
                }
              } else {
                cerr << "ERROR: two registers input, one int/label and two comma should follow beq" <<endl;
                return 1;
              }
              j+=5;
            }

             if((inputLine[i][j].getLexeme() == "bne") && (j==0)){
              if((inputLine[i].size() > 6) && (inputLine[i][6].getKind() != Token::Kind:: COMMENT)&&(inputLine[i][6].getKind() != Token::Kind:: WHITESPACE)) {
                cerr << "ERROR: there should not be value other than comment after bne instruction"<<endl;
                return 1;
              }
              if((j+5<(int)inputLine[i].size())&&(inputLine[i][j+1].getKind() == Token::Kind::REG)
              &&(inputLine[i][j+3].getKind() == Token::Kind::REG)&&((inputLine[i][j+5].getKind() == Token::Kind::HEXINT) || 
              (inputLine[i][j+5].getKind() == Token::Kind::INT))&&(inputLine[i][j+2].getKind() == Token::Kind::COMMA)
              &&(inputLine[i][j+4].getKind() == Token::Kind::COMMA)) {
                int mips = inputLine[i][j+1].toNumber();
                int mips1 = inputLine[i][j+3].toNumber();
                int mips2 = inputLine[i][j+5].toNumber();
                int64_t mips5 = 65535;
                if((mips > 31) || (mips < 0) || (mips1 > 31) || (mips1 < 0)||((inputLine[i][j+5].getKind() == Token::Kind::INT)&&(mips2 > 32767)) 
                ||((inputLine[i][j+5].getKind() == Token::Kind::INT)&&(mips2 <  -32768))
                ||((inputLine[i][j+5].getKind() == Token::Kind::HEXINT)&&(mips2 <  0))
                ||((inputLine[i][j+5].getKind() == Token::Kind::HEXINT)&&(mips2 > 0xffff))) {
                  cerr << "ERROR: the register value should be between 0 and 31 and the int must be in the range"<<endl;
                  return 1;
                } else {
                  int mips4 = mips2&mips5;
                  int mips3 = 5 << 26 | mips << 21 | mips1 << 16 | mips4;
                  printWORD(mips3);
                }
              } else if ((j+5<(int)inputLine[i].size())&&(inputLine[i][j+1].getKind() == Token::Kind::REG)
              &&(inputLine[i][j+3].getKind() == Token::Kind::REG)
              &&(inputLine[i][j+2].getKind() == Token::Kind::COMMA)&&(inputLine[i][j+4].getKind() == Token::Kind::COMMA)) {
                if(symbolTable.count(inputLine[i][j + 5].getLexeme()) == 1) {
                    int mips = inputLine[i][j+1].toNumber();
                    int mips1 = inputLine[i][j+3].toNumber();
                    int mips2 = (int)symbolTable.find(inputLine[i][j + 5].getLexeme())->second;
                    int mips4 = (mips2-memory2-4)/4;
                    int64_t mips5 = 65535;
                    int mips6 = mips4&mips5;
                    int mips3 = 5 << 26 | mips << 21 | mips1 << 16 | mips6;
                    printWORD(mips3);
                } else {
                  cerr << "ERROR: two registers input, one int/label and two comma should follow bne, label not valid" <<endl;
                  return 1;
                }
              } else{
                cerr << "ERROR: two registers input, one int and two comma should follow bne" <<endl;
                return 1;
              }
              j+=5;
            }

            if((inputLine[i][j].getLexeme() == "lis")&& (j==0)){
              if((inputLine[i].size() > 2) && (inputLine[i][2].getKind() != Token::Kind:: COMMENT)&&(inputLine[i][2].getKind() != Token::Kind:: WHITESPACE)) {
                cerr << "ERROR: there should not be value other than comment after lis instruction"<<endl;
                return 1;
              }
              if((j+1<(int)inputLine[i].size())&&(inputLine[i][j+1].getKind() == Token::Kind::REG)) {
                int mips =  inputLine[i][j+1].toNumber();
                if((mips > 31) || (mips < 0)) {
                  cerr << "ERROR: the register value should be between 0 and 31"<<endl;
                  return 1;
                } else {
                  int mips2 = 0 << 16 | mips << 11 | 20;
                  printWORD(mips2);
                }
              }else{
                cerr << "ERROR: a register input should follow lis" <<endl;
                return 1;
              }
              j++;
            }

            if((inputLine[i][j].getLexeme() == "mfhi")&& (j==0)){
              if((inputLine[i].size() > 2) && (inputLine[i][2].getKind() != Token::Kind:: COMMENT)&&(inputLine[i][2].getKind() != Token::Kind:: WHITESPACE)) {
                cerr << "ERROR: there should not be value other than comment after mfhi instruction"<<endl;
                return 1;
              }
              if((j+1<(int)inputLine[i].size())&&(inputLine[i][j+1].getKind() == Token::Kind::REG)) {
                int mips =  inputLine[i][j+1].toNumber();
                if((mips > 31) || (mips < 0)) {
                  cerr << "ERROR: the register value should be between 0 and 31"<<endl;
                  return 1;
                } else {
                  int mips2 = 0 << 16 | mips << 11 | 16;
                  printWORD(mips2);
                }
              }else{
                cerr << "ERROR: a register input should follow mfhi" <<endl;
                return 1;
              }
              j++;
            }

            if((inputLine[i][j].getLexeme() == "mflo")&& (j==0)){
              if((inputLine[i].size() > 2) && (inputLine[i][2].getKind() != Token::Kind:: COMMENT)&&(inputLine[i][2].getKind() != Token::Kind:: WHITESPACE)) {
                cerr << "ERROR: there should not be value other than comment after mflo instruction"<<endl;
                return 1;
              }
              if((j+1<(int)inputLine[i].size())&&(inputLine[i][j+1].getKind() == Token::Kind::REG)) {
                int mips =  inputLine[i][j+1].toNumber();
                if((mips > 31) || (mips < 0)) {
                  cerr << "ERROR: the register value should be between 0 and 31"<<endl;
                  return 1;
                } else {
                  int mips2 = 0 << 16 | mips << 11 | 18;
                  printWORD(mips2);
                }
              }else{
                cerr << "ERROR: a register input should follow mfhi" <<endl;
                return 1;
              }
              j++;
            }

            if((inputLine[i][j].getLexeme() == "mult")&& (j==0)){
              cerr << inputLine[i].size();
            if((inputLine[i].size() > 4) && (inputLine[i][4].getKind() != Token::Kind:: COMMENT)&&(inputLine[i][4].getKind() != Token::Kind:: WHITESPACE)) {
                cerr << "ERROR: there should not be value other than comment after mult instruction"<<endl;
                return 1;
              }
              if((j+3<(int)inputLine[i].size())&&(inputLine[i][j+1].getKind() == Token::Kind::REG)
              &&(inputLine[i][j+3].getKind() == Token::Kind::REG)&&(inputLine[i][j+2].getKind() == Token::Kind::COMMA)) {
                int mips = inputLine[i][j+1].toNumber();
                int mips1 = inputLine[i][j+3].toNumber();
                if((mips > 31) || (mips < 0) || (mips1 > 31) || (mips1 < 0)) {
                  cerr << "ERROR: the register value should be between 0 and 31"<<endl;
                  return 1;
                } else {
                  int mips3 = 0 << 26 | mips << 21 | mips1 << 16 | 24<< 0;
                  printWORD(mips3);
                }
              }else{
                cerr << "ERROR: three register input should follow mult" <<endl;
                return 1;
              }
              j+=3;
            }

            if((inputLine[i][j].getLexeme() == "multu")&& (j==0)){
            if((inputLine[i].size() > 4) && (inputLine[i][4].getKind() != Token::Kind:: COMMENT)&&(inputLine[i][4].getKind() != Token::Kind:: WHITESPACE)) {
                cerr << "ERROR: there should not be value other than comment after multu instruction"<<endl;
                return 1;
              }
              if((j+3<(int)inputLine[i].size())&&(inputLine[i][j+1].getKind() == Token::Kind::REG)
              &&(inputLine[i][j+3].getKind() == Token::Kind::REG)&&(inputLine[i][j+2].getKind() == Token::Kind::COMMA)) {
                int mips = inputLine[i][j+1].toNumber();
                int mips1 = inputLine[i][j+3].toNumber();
                if((mips > 31) || (mips < 0) || (mips1 > 31) || (mips1 < 0)) {
                  cerr << "ERROR: the register value should be between 0 and 31"<<endl;
                  return 1;
                } else {
                  int mips3 = 0 << 26| mips << 21 | mips1 << 16 | 25<< 0;
                  printWORD(mips3);
                }
              }else{
                cerr << "ERROR: three register input should follow multu" <<endl;
                return 1;
              }
              j+=3;
            }

            if((inputLine[i][j].getLexeme() == "div")&& (j==0)){
            if((inputLine[i].size() > 4) && (inputLine[i][4].getKind() != Token::Kind:: COMMENT)&&(inputLine[i][4].getKind() != Token::Kind:: WHITESPACE)) {
                cerr << "ERROR: there should not be value other than comment after multu instruction"<<endl;
                return 1;
              }
              if((j+3<(int)inputLine[i].size())&&(inputLine[i][j+1].getKind() == Token::Kind::REG)
              &&(inputLine[i][j+3].getKind() == Token::Kind::REG)&&(inputLine[i][j+2].getKind() == Token::Kind::COMMA)) {
                int mips = inputLine[i][j+1].toNumber();
                int mips1 = inputLine[i][j+3].toNumber();
                if((mips > 31) || (mips < 0) || (mips1 > 31) || (mips1 < 0)) {
                  cerr << "ERROR: the register value should be between 0 and 31"<<endl;
                  return 1;
                } else {
                  int mips3 = 0 << 26 | mips << 21 | mips1 << 16| 26<< 0;
                  printWORD(mips3);
                }
              }else{
                cerr << "ERROR: three register input should follow div" <<endl;
                return 1;
              }
              j+=3;
            }

            if((inputLine[i][j].getLexeme() == "divu")&& (j==0)){
            if((inputLine[i].size() > 4) && (inputLine[i][4].getKind() != Token::Kind:: COMMENT)&&(inputLine[i][4].getKind() != Token::Kind:: WHITESPACE)) {
                cerr << "ERROR: there should not be value other than comment after divu instruction"<<endl;
                return 1;
              }
              if((j+3<(int)inputLine[i].size())&&(inputLine[i][j+1].getKind() == Token::Kind::REG)
              &&(inputLine[i][j+3].getKind() == Token::Kind::REG)&&(inputLine[i][j+2].getKind() == Token::Kind::COMMA)) {
                int mips = inputLine[i][j+1].toNumber();
                int mips1 = inputLine[i][j+3].toNumber();
                if((mips > 31) || (mips < 0) || (mips1 > 31) || (mips1 < 0)) {
                  cerr << "ERROR: the register value should be between 0 and 31"<<endl;
                  return 1;
                } else {
                  int mips3 = 0 << 26 | mips << 21 | mips1 << 16 | 27<< 0;
                  printWORD(mips3);
                }
              }else{
                cerr << "ERROR: three register input should follow divu" <<endl;
                return 1;
              }
              j+=3;
            }

            if((inputLine[i][j].getLexeme() == "lw")&& (j==0)){
              if((inputLine[i].size() > 7) && (inputLine[i][7].getKind() != Token::Kind:: COMMENT)&&(inputLine[i][7].getKind() != Token::Kind:: WHITESPACE)) {
                cerr << "ERROR: there should not be value other than comment after lw instruction"<<endl;
                return 1;
              }
              if((j+6<(int)inputLine[i].size())&&(inputLine[i][j+1].getKind() == Token::Kind::REG)
              &&(inputLine[i][j+5].getKind() == Token::Kind::REG)&&((inputLine[i][j+3].getKind() == Token::Kind::HEXINT) || 
              (inputLine[i][j+3].getKind() == Token::Kind::INT))&&(inputLine[i][j+2].getKind() == Token::Kind::COMMA)
              &&(inputLine[i][j+4].getKind() == Token::Kind::LPAREN) &&(inputLine[i][j+6].getKind() == Token::Kind::RPAREN)) {
                int mips = inputLine[i][j+1].toNumber();
                int mips1 = inputLine[i][j+3].toNumber();
                int mips2 = inputLine[i][j+5].toNumber();
                int64_t mips5 = 65535;
                int mips4 = mips1&mips5;
                if((mips > 31) || (mips < 0) || (mips2 > 31) || (mips2 < 0)
                ||((inputLine[i][j+3].getKind() == Token::Kind::INT)&&(mips1 > 32767))
                ||((inputLine[i][j+3].getKind() == Token::Kind::INT)&& (mips1 <  -32768))
                ||((inputLine[i][j+3].getKind() == Token::Kind::HEXINT)&& (mips1 <  0))
                ||((inputLine[i][j+3].getKind() == Token::Kind::HEXINT)&& (mips1 >  0xffff))){
                  cerr << "ERROR: the register value should be between 0 and 31 and the int must be in the range"<<endl;
                  return 1;
                } else {
                  int mips3 = 35 << 26 | mips2 << 21 | mips << 16 | mips4;
                  printWORD(mips3);
                }
              } else {
                cerr << "ERROR: two registers input, one int/label and two comma should follow lw" <<endl;
                return 1;
              }
              j+=6;
            }

            if((inputLine[i][j].getLexeme() == "sw")&& (j==0)){
              if((inputLine[i].size() > 7) && (inputLine[i][7].getKind() != Token::Kind:: COMMENT)&&(inputLine[i][7].getKind() != Token::Kind:: WHITESPACE)) {
                cerr << "ERROR: there should not be value other than comment after sw instruction"<<endl;
                return 1;
              }
              if((j+6<(int)inputLine[i].size())&&(inputLine[i][j+1].getKind() == Token::Kind::REG)
              &&(inputLine[i][j+5].getKind() == Token::Kind::REG)&&((inputLine[i][j+3].getKind() == Token::Kind::HEXINT) || 
              (inputLine[i][j+3].getKind() == Token::Kind::INT))&&(inputLine[i][j+2].getKind() == Token::Kind::COMMA)
              &&(inputLine[i][j+4].getKind() == Token::Kind::LPAREN) &&(inputLine[i][j+6].getKind() == Token::Kind::RPAREN)) {
                int mips = inputLine[i][j+1].toNumber();
                int mips1 = inputLine[i][j+3].toNumber();
                int mips2 = inputLine[i][j+5].toNumber();
                int64_t mips5 = 65535;
                int mips4 = mips1&mips5;
                if((mips > 31) || (mips < 0) || (mips2 > 31) || (mips2 < 0)||(mips1 > 32767) || (mips1 < -32768 )) {
                  cerr << "ERROR: the register value should be between 0 and 31 and the int must be in the range"<<endl;
                  return 1;
                } else {
                  int mips3 = 43 << 26 | mips2 << 21 | mips << 16 | mips4;
                  printWORD(mips3);
                }
              } else {
                cerr << "ERROR: two registers input, one int/label and two comma should follow sw" <<endl;
                return 1;
              }
              j+=6;
            }
           memory2 += 4;
        } else if (inputLine[i][j].getKind() == Token::Kind::COMMENT) {
          pop_front(inputLine[i]);
          j--;
        } else if (inputLine[i][j].getKind() == Token::Kind::WHITESPACE) {
          pop_front(inputLine[i]);
          j--;
        } else if (inputLine[i][j].getKind() == Token::Kind::LABEL) {
          pop_front(inputLine[i]);
          j--;
        } else {
             cerr <<"ERROR: invalid MIPS program: must be labels, instruction, or comment" << std::endl;
             return 1;
        }
      }
    }
  } catch (ScanningFailure &f) {
    cerr << f.what() << std::endl;
    return 1;
  }
  // You can add your own catch clause(s) for other kinds of errors.
  // Throwing exceptions and catching them is the recommended way to
  // handle errors and terminate the program cleanly in C++. Do not
  // use the std::exit function, which may leak memory.
  
  return 0;
}
