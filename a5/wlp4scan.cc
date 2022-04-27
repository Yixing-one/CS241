#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <sstream>
#include <map>
#include <string>
#include <math.h> 
#include <set>
#include <cstdint>
#include <ostream>
using namespace std;

class Token;
std::vector<Token> scan(const std::string &input);
class Token {
  public:
    enum Kind {
    ID,
    NUM,
    LPAREN,
    RPAREN,
    LBRACE,
    RBRACE,
    RETURN,
    IF,
    ELSE,
    WHILE,
    PRINTLN,
    WAIN,
    BECOMES,
    INT,
    EQ,
    NE,
    LT,
    GT,
    LE,
    GE,
    PLUS,
    MINUS,
    STAR,
    SLASH,
    PCT,
    COMMA,
    SEMI,
    NEW,
    DELETE,
    LBRACK,
    RBRACK,
    AMP,
    STR_NULL,
    COMMENT,
    WHITESPACE
    };

  private:
    Kind kind;
    std::string lexeme;

  public:
    Token(Kind kind, std::string lexeme);
    Kind getKind() const;
    const std::string &getLexeme() const;
};

std::ostream &operator<<(std::ostream &out, const Token &tok);

class ScanningFailure {
    std::string message;

  public:
    ScanningFailure(std::string message);
    const std::string &what() const;
};

Token::Token(Token::Kind kind, std::string lexeme):
  kind(kind), lexeme(std::move(lexeme)) {}

Token:: Kind Token::getKind() const { return kind; }
const std::string &Token::getLexeme() const { return lexeme; }

std::ostream &operator<<(std::ostream &out, const Token &tok) {
  switch (tok.getKind()) {
    case Token::ID:           out << "ID";         break;
    case Token::NUM:          out << "NUM";        break;
    case Token::LPAREN:       out << "LPAREN";     break;
    case Token::RPAREN:       out << "RPAREN";     break;
    case Token::LBRACE:       out << "LBRACE";     break;
    case Token::RBRACE:       out << "RBRACE";     break;
    case Token::RETURN:       out << "RETURN";     break;
    case Token::IF:           out << "IF";         break;
    case Token::ELSE:         out << "ELSE";       break;
    case Token::WHILE:        out << "WHILE";      break;
    case Token::PRINTLN:      out << "PRINTLN";    break;
    case Token::WAIN:         out << "WAIN";       break;
    case Token::BECOMES:      out << "BECOMES";    break;
    case Token::INT:          out << "INT";        break;
    case Token::EQ:           out << "EQ";         break;
    case Token::NE:           out << "NE";         break;
    case Token::LT:           out << "LT";         break;
    case Token::GT:           out << "GT";         break;
    case Token::LE:           out << "LE";         break;
    case Token::GE:           out << "GE";         break;
    case Token::PLUS:         out << "PLUS";       break;
    case Token::MINUS:        out << "MINUS";      break;
    case Token::STAR:         out << "STAR";       break;
    case Token::SLASH:        out << "SLASH";      break;
    case Token::PCT:          out << "PCT";        break;
    case Token::COMMA:         out << "COMMA";      break;
    case Token::SEMI:          out << "SEMI";       break;
    case Token::NEW:          out << "NEW";        break;
    case Token::DELETE:        out << "DELETE";     break;
    case Token::LBRACK:        out << "LBRACK";     break;
    case Token::RBRACK:        out << "RBRACK";     break;
    case Token::AMP:           out << "AMP";        break; 
    case Token::STR_NULL:      out << "NULL";       break; 
    default: throw ScanningFailure("ERROR:space/comment/line/tab should not be printed");
  }
  out << " " << tok.getLexeme() << endl;
  return out;
}

ScanningFailure::ScanningFailure(std::string message):
  message(std::move(message)) {}

const std::string &ScanningFailure::what() const { return message; }

/* Representation of a DFA, used to handle the scanning process.
 */
class AsmDFA {
  public:
    enum State {
     // States that are also kinds
    ID,
    NUM,
    LPAREN,
    RPAREN,
    LBRACE,
    RBRACE,
    RETURN,
    IF,
    ELSE,
    WHILE,
    PRINTLN,
    WAIN,
    BECOMES,
    INT,
    EQ,
    NE,
    LT,
    GT,
    LE,
    GE,
    PLUS,
    MINUS,
    STAR,
    SLASH,
    PCT,
    COMMA,
    SEMI,
    NEW,
    DELETE,
    LBRACK,
    RBRACK,
    AMP,
    STR_NULL,
    COMMENT,
    WHITESPACE,

    // States that are not kinds
    FAIL,
    START,
    STR_R,
    STR_RE,
    STR_RET,
    STR_RETU,
    STR_RETUR,
    STR_P,
    STR_PR,
    STR_PRI,
    STR_PRIN,
    STR_PRINT,
    STR_PRINTL,
    STR_E,
    STR_EL,
    STR_ELS,
    STR_W,
    STR_WH, 
    STR_WHI,
    STR_WHIL,
    STR_WA,
    STR_WAI,
    STR_I,
    STR_IN,
    STR_NOT,
    STR_n,
    STR_nE,
    STR_D,
    STR_DE,
    STR_DEL,
    STR_DELE,
    STR_DELET,
    STR_N,
    STR_NU,
    STR_NUL,

    LARGEST_STATE = STR_NUL
    };

  private:
    /* A set of all accepting states for the DFA.
     * Currently non-accepting states are not actually present anywhere
     * in memory, but a list can be found in the constructor.
     */
    std::set<State> acceptingStates;
    std::set<State> notacceptingStates;

    /*
     * The transition function for the DFA, stored as a map.
     */

    std::array<std::array<State, 128>, LARGEST_STATE + 1> transitionFunction;

    /*
     * Converts a state to a kind to allow construction of Tokens from States.
     * Throws an exception if conversion is not possible.
     */
    Token::Kind stateToKind(State s) const {
      switch(s) {
        case ID:          return Token::ID;
        case NUM:         return Token::NUM;
        case LPAREN:      return Token::LPAREN;
        case RPAREN:      return Token::RPAREN;
        case LBRACE:      return Token::LBRACE;
        case RBRACE:      return Token::RBRACE;
        case RETURN:      return Token::RETURN;
        case IF:          return Token::IF;
        case ELSE:        return Token::ELSE;
        case WHILE:       return Token::WHILE;
        case PRINTLN:     return Token::PRINTLN;
        case WAIN:        return Token::WAIN;
        case BECOMES:     return Token::BECOMES;
        case INT:         return Token::INT;
        case EQ:          return Token::EQ;
        case NE:          return Token::NE;
        case LT:          return Token::LT;
        case GT:          return Token::GT;
        case LE:          return Token::LE;
        case GE:          return Token::GE;
        case PLUS:        return Token::PLUS;
        case MINUS:       return Token::MINUS;
        case STAR:        return Token::STAR;
        case SLASH:       return Token::SLASH;
        case PCT:         return Token::PCT;
        case COMMA:       return Token::COMMA;
        case SEMI:        return Token::SEMI;
        case NEW:         return Token::NEW;
        case DELETE:      return Token::DELETE;
        case LBRACK:      return Token::LBRACK;
        case RBRACK:      return Token::RBRACK;
        case AMP:         return Token::AMP;
        case STR_NULL:    return Token::STR_NULL;
        case COMMENT:     return Token::COMMENT;
        case WHITESPACE:     return Token::WHITESPACE;
        default: throw ScanningFailure("ERROR: Cannot convert state to kind.");
      }
    }


  public:
    /* Initializes the accepting states for the DFA.
     */
    AsmDFA() {
      acceptingStates = {ID,NUM, LPAREN, RPAREN, LBRACE, RBRACE, RETURN,
                          IF, ELSE, WHILE, PRINTLN, WAIN, BECOMES, INT, EQ,
                          NE, LT, GT, LE, GE, PLUS, MINUS, STAR, SLASH, PCT, COMMA,
                          SEMI, NEW, DELETE, LBRACK, RBRACK, AMP, STR_NULL, COMMENT,WHITESPACE};
      notacceptingStates = {STR_R,STR_RE, STR_RET, STR_RETU, STR_RETUR,
                                  STR_P,STR_PR,STR_PRI,STR_PRIN,STR_PRINT,STR_PRINTL,
                                  STR_E,STR_EL, STR_ELS,STR_W,STR_WA,STR_WAI,
                                  STR_I,STR_IN,STR_N,STR_D,STR_DE,STR_DEL,STR_DELE,
                                  STR_DELET,STR_NU,STR_NUL,STR_WH, STR_WHI,STR_WHIL,STR_n,STR_nE};
      // Initialize transitions for the DFA
      for (size_t i = 0; i < transitionFunction.size(); ++i) {
        for (size_t j = 0; j < transitionFunction[0].size(); ++j) {
          transitionFunction[i][j] = FAIL;
        }
      }
      string c = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
      registerTransition(START, "\t\r\n", WHITESPACE);
      registerTransition(WHITESPACE, "\t\r\n", WHITESPACE);
      registerTransition(START, " ", WHITESPACE);
      registerTransition(WHITESPACE, " ", WHITESPACE);
      registerTransition(START, "0123456789", NUM);
      registerTransition(NUM, "0123456789", NUM);
      registerTransition(START, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz", ID);
      registerTransition(ID, c, ID);
      registerTransition(START, "(", LPAREN);
      registerTransition(START, ")", RPAREN);
      registerTransition(START, "{", LBRACE);
      registerTransition(START, "}", RBRACE);
      registerTransition(START, "r", STR_R);
      registerTransition(STR_R, "e", STR_RE);
      registerTransition(STR_RE, "t", STR_RET);
      registerTransition(STR_RET, "u", STR_RETU);
      registerTransition(STR_RETU, "r", STR_RETUR);
      registerTransition(STR_RETUR, "n", RETURN);
      registerTransition(RETURN, c, ID);
      registerTransition(START, "i", STR_I);
      registerTransition(STR_I, "f", IF);
      registerTransition(START, "e", STR_E);
      registerTransition(STR_E, "l", STR_EL);
      registerTransition(STR_EL, "s", STR_ELS);
      registerTransition(STR_ELS, "e", ELSE);
      registerTransition(ELSE, c, ID);
      registerTransition(START, "p", STR_P);
      registerTransition(STR_P, "r", STR_PR);
      registerTransition(STR_PR, "i", STR_PRI);
      registerTransition(STR_PRI, "n", STR_PRIN);
      registerTransition(STR_PRIN, "t", STR_PRINT);
      registerTransition(STR_PRINT, "l", STR_PRINTL);
      registerTransition(STR_PRINTL, "n", PRINTLN);
      registerTransition(PRINTLN, c, ID);
      registerTransition(START, "w", STR_W);
      registerTransition(STR_W, "h", STR_WH);
      registerTransition(STR_WH, "i", STR_WHI);
      registerTransition(STR_WHI, "l", STR_WHIL);
      registerTransition(STR_WHIL, "e", WHILE);
      registerTransition(WHILE, c, ID);
      registerTransition(STR_W, "a", STR_WA);
      registerTransition(STR_WA, "i", STR_WAI);
      registerTransition(STR_WAI, "n", WAIN);
      registerTransition(START, "=", BECOMES);
      registerTransition(START, "i", STR_I);
      registerTransition(STR_I, "n", STR_IN);
      registerTransition(STR_IN, "t", INT);
      registerTransition(INT, c, ID);
      registerTransition(BECOMES, "=", EQ);
      registerTransition(START, "!", STR_NOT);
      registerTransition(STR_NOT, "=", NE);
      registerTransition(START, "<", LT);
      registerTransition(START, ">", GT);
      registerTransition(LT, "=", LE);
      registerTransition(GT, "=", GE);
      registerTransition(START, "+", PLUS);
      registerTransition(START, "-", MINUS);
      registerTransition(START, "*",STAR);
      registerTransition(START, "/",SLASH);
      registerTransition(START, "%",PCT);
      registerTransition(START, ",",COMMA);
      registerTransition(START, ";",SEMI);
      registerTransition(START, "n",STR_n);
      registerTransition(STR_n, "e",STR_nE);
      registerTransition(STR_nE, "w",NEW);
      registerTransition(NEW, c, ID);
      registerTransition(START, "d",STR_D);
      registerTransition(STR_D, "e",STR_DE);
      registerTransition(STR_DE, "l",STR_DEL);
      registerTransition(STR_DEL, "e",STR_DELE);
      registerTransition(STR_DELE, "t",STR_DELET);
      registerTransition(STR_DELET, "e",DELETE);
      registerTransition(DELETE, c, ID);
      registerTransition(START, "[",LBRACK);
      registerTransition(START, "]",RBRACK);
      registerTransition(START, "&",AMP);
      registerTransition(START, "N",STR_N);
      registerTransition(STR_N, "U",STR_NU);
      registerTransition(STR_NU, "L",STR_NUL);
      registerTransition(STR_NUL, "L", STR_NULL);
      registerTransition(STR_NULL, c, ID);
      registerTransition(SLASH, "/", COMMENT);
    }

    // Register a transition on all chars in chars
    void registerTransition(State oldState, const std::string &chars,
        State newState) {
      for (char c : chars) {
        transitionFunction[oldState][c] = newState;
      }
    }

    // Register a transition on all chars matching test
    // For some reason the cctype functions all use ints, hence the function
    // argument type.
    void registerTransition(State oldState, int (*test)(int), State newState) {

      for (int c = 0; c < 128; ++c) {
        if (test(c)) {
          transitionFunction[oldState][c] = newState;
        }
      }
    }

    /* Returns the state corresponding to following a transition
     * from the given starting state on the given character,
     * or a special fail state if the transition does not exist.
     */
    State transition(State state, char nextChar) const {
      return transitionFunction[state][nextChar];
    }

    /* Checks whether the state returned by transition
     * corresponds to failure to transition.
     */
    bool failed(State state) const { return state == FAIL; }

    /* Checks whether the state returned by transition
     * is an accepting state.
     */
    bool accept(State state) const {
      return acceptingStates.count(state) > 0;
    }

    /* Returns the starting state of the DFA
     */
    State start() const { return START; }

std::vector<Token> simplifiedMaximalMunch(const std::string &input) const {
  std::vector<Token> newTokens;
  if(input.size() == 0) {
    return newTokens;
  }
  State state = start();
  std::string stringInput;
  for (std::string::const_iterator inputPosn = input.begin(); inputPosn != input.end();) { 
          State oldState = state;  
          state = transition(state, *inputPosn);
          if(state == COMMENT) {
            stringInput += *inputPosn;
            newTokens.push_back(Token(stateToKind(state), stringInput));
            return newTokens;
          }
          if (!failed(state)) {
            stringInput += *inputPosn;
            oldState = state;
            ++inputPosn;
          }
          if (failed(state)) {
               if (notacceptingStates.count(oldState) > 0) {
                oldState = ID;
                state = transition(oldState, *inputPosn);
               }               
          }
          if (inputPosn == input.end() || failed(state)) {
            if (accept(oldState)) {
             newTokens.push_back(Token(stateToKind(oldState), stringInput));
             stringInput = "";
             state = start();
          }else {
            if (failed(state)) {
              stringInput += *inputPosn;
            }
            throw ScanningFailure("ERROR: Simplified maximal munch failed on input: "
                                 + stringInput);
          }
      }
    }
    return newTokens;
  }
};

std::vector<Token> scan(const std::string &input) {
  static AsmDFA theDFA;
  std::vector<Token> tokens = theDFA.simplifiedMaximalMunch(input);
  std::vector<Token> newTokens;
  for (auto &token : tokens) {
    if (token.getKind() != Token::WHITESPACE
        && token.getKind() != Token::Kind::COMMENT) {
      newTokens.push_back(token);
    }
  }
  return newTokens;
}

int main() {
  std::string line;
  try {
    while (getline(std::cin, line)) {
      std::vector<Token> tokenLine = scan(line);
      for(int i = 0; i < (int)tokenLine.size(); i++) {
        if(tokenLine[i].getKind() == Token::Kind::NUM) {
          int j = 0;
          string s = tokenLine[i].getLexeme();
          stringstream ss;
          ss << s; 
          ss >> j; 
          if (j > (int)2147483647){
            cerr << "ERROR: input int exceed max" << endl;
            return 1;
          }
          if((s[0] == '0')&&(s.length() > 1)) {
            cerr << "ERROR: leading 0" << endl;
            return 1;
          }
        }
        cout << tokenLine[i];
      }
    }
    } catch (ScanningFailure &f) {
    cerr << f.what() << std::endl;
    return 1;
    }
  }