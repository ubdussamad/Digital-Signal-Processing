#include "SignalProcessingUtil.hpp"

template < typename T >
void dsp <T> :: LOG ( T message , short level = 3) {
    if (enableDebugLog || (level == 1)) {
    std::cout << "Level: 3 | Info: " << message << std::endl;
    }
}


template < typename  T >
dsp <T> ::dsp ( const doubleVector_t&  signal ) {
    signalPointer = new doubleVector_t;
    LOG ("Loaded Double Vector Signal.")

}

template < typename  T >
dsp <T> :: dsp ( T* signal , int length  ) {
    
}