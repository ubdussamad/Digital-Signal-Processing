#include "SignalProcessingUtil.hpp"


template < typename signalArraytype>
void dsp<signalArraytype>::LOG ( std::string message , short level ) {
    if (level == 1 || enableDebugLog ) {
        std::cout << std::time(0) << ": " << message << std::endl;
    }
}


template < typename signalArraytype>
dsp <signalArraytype> :: dsp( const complexSignalVector_t<signalArraytype>& signal ) :
    inputSignal(signal) {
        LOG("SignalObj Instanciated, type: Complex Vector <T> " , 3);
}

template < typename signalArraytype>
dsp <signalArraytype> :: dsp( const signalVector_t<signalArraytype>& signal ) {
    
}

template < typename signalArraytype >
complexSignalVector_t<signalArraytype> dsp <signalArraytype> :: dft ( const signalArraytype sweepDelta ) {
    const double N = inputSignal.size();
    double F = N;
    complexSignalVector_t<signalArraytype> output;
    output.reserve(N);

    for ( signalArraytype f = 0 ; f < F ; f+=sweepDelta ) {
        std::complex < signalArraytype > tmp(0,0);

        for ( signalArraytype  n = 0; n < N ; n++ ) {
            double realMul = cos((2*M_PI*f*n)/N);
            double imagMul = sin((2*M_PI*f*n)/N);
            std::complex < signalArraytype > z ( realMul , -imagMul );
            tmp += array[n] * z;
        }

        output.push_back( tmp * dft_scaling_factor );
    }

    return ( output );
}


int main () {
    complexSignalVector_t<double> siG;
    dsp<double> x( siG );
    return(0);
}