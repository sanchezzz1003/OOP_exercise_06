/*
 * Комков Александр М8О-201Б
 * Вариант 4
 * Стек
 * Стек
 * Трапеция
 */

#include <algorithm>
#include <iostream>
#include "stack.h"
#include "trapeze.h"
#include "myallocator.h"

template <class T>
void Trapeze_read_cords(std::pair<T, T> &A, std::pair<T, T> &B, std::pair<T, T> &C, std::pair<T, T> &D){
    std::cout << "cords point A: ";
    std::cin >> A;
    std::cout << "cords point B: ";
    std::cin >> B;
    std::cout << "cords point C: ";
    std::cin >> C;
    std::cout << "cords point D: ";
    std::cin >> D;
}

int main() {

    int index, val;
    size_t const memsize;
    std::cout << "The size of one Trapeze in the stack is 56" << std::endl;
    std::cin >> memsize;
    Stack<Trapeze<int>,my_allocator<Trapeze<int>, 100000>> t;
    std::string option = "Menu";
    while(option != "Ex"){
        if (option == "Add") {
            try{
                std::cout << "Add an item to the top of the stack[Push] or to the iterator position[Iter]" << std::endl;
                std::cin >> option;
                std::pair<int, int> A, B, C, D;
                Trapeze_read_cords(A, B, C, D);
                Trapeze<int> Trap(A, B, C, D);
                if (option == "Push") {
                    t.Push(Trap);
                } else if (option == "Iter") {
                    std::cout << "Enter the index of the item, after which insert the item" << std::endl;
                    std::cin >> index;
                    try {
                        auto it = t.begin();
                        for (int i = 0; i < index-1; ++i) {
                            ++it;
                        }
                        t.insert(it, Trap);
                    } catch (std::logic_error &e) {
                        std::cout << e.what() << std::endl;
                        continue;
                    }
                }
            }catch (std::bad_alloc& e){
                std::cout << e.what() << std::endl;
                std::cout << "Memory limit\n";
                continue;
                }
        } else if(option == "Rem") {
            std::cout << "Delete item on top of stack[Pop] or to the iterator position[Iter]" << std::endl;
            std::cin >> option;
            if (option == "Pop") {
                try {
                    t.Pop();
                } catch (std::logic_error &e) {
                    std::cout << e.what() << std::endl;
                    continue;
                }
            } else if (option == "Iter") {
                std::cout << "Enter the index of the item to be deleted" << std::endl;
                std::cin >> index;
                try {
                    if (index < 0 || index > t.Size) {
                        throw std::logic_error("Out of bounds\n");
                    }

                    auto it = t.begin();
                    for (int i = 0; i < index-1; ++i) {
                        it++;
                    }
                    t.erase(it);
                } catch (std::logic_error &e) {
                    std::cout << e.what() << std::endl;
                    continue;
                }
            }
        }else if(option == "print") {

            int num = 1;
            std::for_each(t.begin(), t.end(), [&num](auto e) {
                std::cout << num << ". " << std::endl;
                e.Coord_print(std::cout);
                std::cout << "Trapeze area " << e.area_ << std::endl;
                num++;
            });
        } else if(option == "Top") {
            try {
                t.Top().Coord_print(std::cout);
            } catch (std::logic_error &e) {
                std::cout << e.what() << std::endl;
                continue;
            }
        } else if(option == "count_if"){
            std::cout << "Enter area" << std::endl;
            std::cin >> val;
            std::cout << "The number of figures with an area less than a given "<< std::count_if(t.begin(),t.end(), [val](Trapeze<int> Rec)
            {return Rec.area_ < val;})  << std::endl;
        } else if (option == "Menu") {
            std::cout << "Enter a word for action:" << std::endl;
            std::cout << "1) Add Trapeze to the stack[Add]" <<
                      std::endl << "2) Remove item [Rem]" <<
                      std::endl << "3) Show all shapes with std::for_each [print]" <<
                      std::endl << "4) Show top stack element [Top]" <<
                      std::endl << "5) Display on the screen the number of objects whose area is"
                                   " less than the specified with std::count_if [count_if]" <<
                      std::endl << "6) Print Menu[Menu]" <<
                      std::endl << "7) Exit[Ex]" << std::endl;
        } else {
            std::cout << "You did not select an action or you were mistaken in entering the name of the action" << std::endl;
        }
        std::cin >> option;
    }
    return 0;
}
