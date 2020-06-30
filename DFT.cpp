#include <iostream>
#include <vector>
#include <complex>
#include <math.h>


typedef std::vector <std::complex <double>> complexVector;
typedef std::complex <double> complexDouble;

complexVector dft ( complexVector array ) {
    double N = array.size();
    double F = N;

    complexVector output;
    output.reserve(N);

    double deltaF = 1;
    for ( double f = 0 ; f < F ; f+deltaF ) {
        complexDouble tmp(0,0);

        for ( double  n = 0; n < N ; n++ ) {
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

    double signalLength = 1000;
    complexVector signal;
    signal.reserve(signalLength);
    double signalFrequency = 3.000;
    double signalPhase = 0.000;//M_PI / 4;

    for ( int i= 0 ; i < signalLength ; i++ ) {
        double realP = cos ( ( ((2 * M_PI)/ signalLength) * (signalFrequency * static_cast<double>(i)) ) +  signalPhase  );
        signal.push_back ( complexDouble ( realP, 0.000 ) );
    }

    complexVector result = dft ( signal );

    for ( int i = 0; i<6 ; i++ ) {
        
    }

    return(0);


}