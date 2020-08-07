#include "SignalProcessingUtil.hpp"

template < typename SignalType >
void dsp <SignalType> :: LOG ( std::string message , short level ) {
    if ( this->enableDebugLog || (level == 1)) {
    std::cout << "Level: 3 | Info: " << message << std::endl;
    }
}


// template < typename  T >
// dsp <T> ::dsp ( const doubleVector_t&  signal ) {
//     signalPointer = new doubleVector_t;
//     LOG ("Loaded Double Vector Signal.")

// }

template < typename  T >
dsp <T> :: dsp ( T* signal , T length  ) {
    signalPointer = new complexVector_t;
    signalPointer->reserve(length);
    for ( T i = 0 ; i < length ; i++ ) {
        signalPointer->push_back (*signal);
        // LOG ( *signal , 1);
        std::cout << *signal << std::endl;
        signal++;
    }
    // LOG ("Loaded Double Vector Signal." , 1);
    std::cout << "Loaded Double Vector" << std::endl;

}


int main ( void ) {
    std::cout << "Initializing......" << std::endl;
    float list[5] = { 1, 2, 3, 4, 5};
    auto instance = dsp<float>( list , 5 );
    
    return(0);

}


