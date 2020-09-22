#include <iostream>



class Base {
    public:
        int a;
    protected:
        int b;
    private:
        int c;
};
/* this is is class and it s is fervied form a base class. */
class Derived : public Base {
    
    int getA ( void ) {
        return this->a;
    }
    
    int getB ( void ) {
        return this->b;
    }

    int getC (void) {
        return this->c;
    }

    
};

int main ( ) {
    Derived x;
    x.a = 2;
    x.b = 3;
    x.c = 4;
}