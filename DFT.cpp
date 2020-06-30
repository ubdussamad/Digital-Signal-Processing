#include <iostream>
#include <vector>
#include <complex>
#include <math.h>


typedef std::vector <std::complex <double>> complexVector;
typedef std::complex <double> complexDouble;

complexVector fft ( complexVector array ) {
    uint32_t N = array.size();
    uint32_t F = N;

    complexVector output;
    output.reserve(N);

    uint32_t deltaF = 1;
    for ( uint32_t f = 0 ; f < F ; f+deltaF ) {
        complexDouble tmp(0,0);

        for ( uint32_t  n = 0; n < N ; n++ ) {
            double realMul = cos((2*M_PI*f*n)/N);
            double imagMul = sin((2*M_PI*f*n)/N);

            complexDouble z ( realMul , -imagMul );
            tmp += array[n] * z;
        }

        output.push_back(tmp);
    }

    return ( output );
}

int main ( void ) {
    return(0);
}