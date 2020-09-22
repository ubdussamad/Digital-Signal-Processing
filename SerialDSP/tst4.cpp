#include <vector>
#include <cstdlib>
#include <iostream>


class fifoVector : public std::vector<double> {
    public:
        fifoVector (int bufferSize_ = 7);
        void push_back_( const double& x);
    private:
        int bufferSize;
};

fifoVector::fifoVector (int bufferSize_) :
    std::vector<double>(10,12), // Very Experimental
    bufferSize(bufferSize_) {

}

void fifoVector::push_back_( const double& x) {
    std::cout << "Push Called!\n";
    if (this->size() == this->bufferSize) {
        this->erase( this->begin() ); // Remove the first element.
        std::cout << "Buffer Capacity Reached.\n\n";
    }
    this->push_back(x);
}

int main (void) {
    fifoVector x(10);
    // for (size_t i = 1; i < 110; i++)
    // {   
    //     std::cout << "Pushing: " << i << "\n";
    //     x.push_back_(i);
    // }
    std::cout << "done pushing!" << std::endl;
    for (auto i: x)
    {
        std::cout << i << std::endl;
    }
    
    
}



