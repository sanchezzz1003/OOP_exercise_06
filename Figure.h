#ifndef EXERCISE_05_FIGURES_H
#define EXERCISE_05_FIGURES_H
#include <string>
#include <type_traits>
#include <cmath>
#include <stdexcept>
#include <iostream>

template <class T>
std::pair<T, T> operator-(const std::pair<T, T> &p1, const std::pair<T, T> &p2){
    return {p1.first-p2.first, p1.second-p2.second};
}

template <class T>
std::ostream& operator<<(std::ostream &o, const std::pair<T, T> &p){
    o << "<" << p.first << ", " << p.second << ">";
    return o;
}


template <class T>
bool operator==(const std::pair<T, T> &a, const std::pair<T, T> &b){
    return (a.first == b.first) && (a.second == b.second);
}

bool isNumber(const std::string& s){
    return !s.empty() && s.find_first_not_of("-.0123456789") == std::string::npos; }

template <class T>
std::istream& operator>>(std::istream &is, std::pair<T, T> &p){
    std::string checker;

    is >> checker;
    if(isNumber(checker) == false){
        throw std::overflow_error("Is not a number");
    }
    p.first = static_cast<T>(std::stod(checker));

    is >> checker;
    if(isNumber(checker) == false){
        throw std::overflow_error("Is not a number");
    }
    p.second = static_cast<T>(std::stod(checker));

    return is;
}

template <class T>
bool collinear(const std::pair<T, T> &a, const std::pair<T, T> &b, const std::pair<T, T> &c, const std::pair<T, T> &d){
    return (b.second-a.second)*(d.first-c.first) - (d.second-c.second)*(b.first-a.first) <= 1e-9;
}

template <int>
bool collinear(std::pair<int, int> &a, std::pair<int, int> &b, std::pair<int, int> &c, const std::pair<int, int> &d){
    return (b.second-a.second)*(d.first-c.first) == (d.second-c.second)*(b.first-a.first);
}

template <class T>
bool perpendic(const std::pair<T, T> &a, const std::pair<T, T> &b, const std::pair<T, T> &d){
    using vect = std::pair<T, T>;
    vect AB = b-a;
    vect AD = d-a;
    T dotProduct = AB.first*AD.first + AB.second*AD.second;
    if(dotProduct <= 1e-9 && dotProduct >= -1e-9) return true;
    else return false;
}

template <class T>
double dist(const std::pair<T,T> &a, const std::pair<T,T> &b){
    return sqrt( ((b.first - a.first) * (b.first - a.first)) + ((b.second - a.second) * (b.second - a.second)));
}

template <class T>
class Trapeze{
public:
    using type = T;
    using point = std::pair<T, T>;
    point A, B, C, D, H;
    type area_;
    point center();
    void Coord_print(std::ostream& os);

    Trapeze() = default;

    Trapeze(point &a, point &b, point &c, point &d){
        auto AB = dist(a,b);
        auto CD = dist(c,d);
        auto AD = dist(a,d);
        auto BC = dist(b,c);
        if(a == b || a == c || b == c || a == d || 
            (!collinear(a, d, c, b) && !collinear(a, b, d, c))){
            throw std::overflow_error("The entered coordinates of the vertices do not belong to the trapeze.");
        }else{
            A = a;
            B = b;
            C = c;
            D = d;
            if (collinear(A, D, C, B)){
                auto ABD = abs((D.second-A.second)*B.first - (D.first - A.first)*B.second + D.first*A.second - D.second*A.first);
                auto BH = ABD/AD;  
                area_ = (AD+BC)/2*BH;
            }
            else if (collinear (A, B, C, D)){
                auto ABD = abs((D.second-A.second)*B.first - (D.first - A.first)*B.second + D.first*A.second - D.second*A.first);
                auto BH = ABD/AB;
                area_ = (AB+BC)/2*BH; 
            }
        }
    }
    type area(){
        return area_;
    }
    ~Trapeze(){};
};

template <class T>
std::pair<T,T> Trapeze<T>::center(){
    typename T::type Ox, Oy;
    Ox = (A.first + B.first + C.first + D.first)/4;
    Oy = (A.second + B.second + C.second + D.second)/4;
    return {Ox, Oy};
}

template <class T>
void Trapeze<T>::Coord_print(std::ostream& os) {
    std::cout << "Cords of Trapeze: " << A << ", " << B << ", " << C << ", " << D << std::endl;
}

#endif //EXERCISE_05_FIGURES_H
