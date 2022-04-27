#include <iostream>
using namespace std;

//Skip the grammar part of the input.
void skipLine(istream &in) {
  string s;
  getline(in, s);
}
 
void skipGrammar(istream &in) {
  int i, numTerm, numNonTerm, numRules;

  // read the number of terminals and move to the next line
  in >> numTerm;
  skipLine(in);
  
  // skip the lines containing the terminals
  for (i = 0; i < numTerm; i++) {
    skipLine(in);
  }
  
  // read the number of non-terminals and move to the next line
  in >> numNonTerm;
  skipLine(in);
  
  // skip the lines containing the non-terminals
  for (i = 0; i < numNonTerm; i++) {
    skipLine(in);
  }
  
  // skip the line containing the start symbol
  skipLine(in);

  // read the number of rules and move to the next line
  in >> numRules;
  skipLine(in);
  
  // skip the lines containing the production rules
  for (i = 0; i < numRules; i++) {
    skipLine(in);
  }
}

string trim(const string &str) {
  size_t begin = str.find_first_not_of(" \t\n");
  if (begin == string::npos) return "";

  size_t end = str.find_last_not_of(" \t\n");

  return str.substr(begin, end - begin + 1);
}

int find_result(istream &in) {
  string ln, ln2;
  int result1 = 0;
  int result2 = 0;
  getline (in, ln);
  ln2 = trim(ln);
  if(ln2 =="S BOF expr EOF"){
      return find_result(in);
      
  }
  if(ln2 == "expr term") {
      return find_result(in);
  }
  if(ln2 == "expr expr - term") {
      result1 = find_result(in);
      result2 = find_result(in);
      return result1 - result2;
  }
  if(ln2 == "term id") {
      return 42;
  }
  if(ln2 == "term ( expr )") {
      return find_result(in);
  }
  return -1;
}

int main (int argc, char **argv) {
  skipGrammar(std::cin);
  int result;
  result = find_result(cin);
  cout << result << endl;
}