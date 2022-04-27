#include <iostream>
using namespace std;

void traverse(istream &input){
    int node, num_child;
    if(input >> node >> num_child) {
        if (num_child == 0) {
             cout << node <<" " << num_child << endl;
        } else {
            for (int i = 0; i < num_child; i++) {
                traverse(input);
            }
            cout << node <<" " << num_child << endl;
        }
    }
}

int main (int argc, char **argv) {
    traverse(cin);
}