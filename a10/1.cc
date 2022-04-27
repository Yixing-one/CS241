#include<iostream>
#include "a10p2.h"

int main() {
    std::cout << "Let's make a little list" << std::endl;
    int64_t* list = cons(1,cons(2,cons(3,cons(4,cons(5,nullptr)))));

    std::cout << "Get some values and cells" << std::endl;
    int64_t  first_value  = car(list);
    int64_t* second_cell  = cdr(list);
    int64_t* third_cell   = cdr(cdr(list));
    int64_t  fourth_value = car(cdr(third_cell));
    int64_t* fifth_cell   = cdr(cdr(third_cell));
    if(first_value != 1 || fourth_value != 4) { std::exit(1); }

    std::cout << "Now change some values and cells" << std::endl;
    setcar(list,4);                     // first element is now 4
    setcdr(cdr(third_cell),nullptr);    // list now ends after 4 elements
    setcar(third_cell,4);               // third element is now 2
    setcdr(list,cdr(second_cell));      // delete the second cell from the list
    if(car(list) != 4 || car(cdr(list)) != 4 || car(cdr(cdr(list))) != 4) { std::exit(1); }

    std::cout << "Snoc the cells that aren't part of the list anymore" << std::endl;
    snoc(second_cell);
    snoc(fifth_cell);

    std::cout << "Now let's make another list" << std::endl;
    int64_t* lost = cons(241,cons(124,cons(412,nullptr)));
    int64_t* lost_cells[3]  = { lost, cdr(lost), cdr(cdr(lost))};
    int64_t  lost_values[3] = { car(lost), car(cdr(lost)), car(cdr(cdr(lost))) };
    if(lost_values[0] != 241 || lost_values[1] != 124 || lost_values[2] != 412) { std::exit(1); }

    std::cout << "Snoc the cells in a weird order" << std::endl;
    snoc(lost_cells[1]);
    snoc(lost_cells[0]);
    snoc(lost_cells[2]);

    std::cout << "Create the list again" << std::endl;
    lost = cons(241,cons(124,cons(412,nullptr)));

    std::cout << "How's our other list doing?" << std::endl;
    if(car(list) != 4 || car(cdr(list)) != 4 || car(cdr(cdr(list))) != 4) { std::exit(1); }

    std::cout << "Snoc everything!!!" << std::endl;
    int64_t* lists[2] = {list, lost};
    for(int i=0; i<2; i++) {
        while(lists[i]) {
            int64_t* temp = cdr(lists[i]);
            snoc(lists[i]);
            lists[i] = temp;
        }
    }

    std::cout << "Wow, we made it to the end" << std::endl;
    return 0;
}