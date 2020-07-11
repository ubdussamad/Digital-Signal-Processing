#include <iostream>
#include <vector>
#include <complex>
#include <math.h>
#include <map>
#include <cmath>
#include <stdlib.h>
#include "gnuplot-iostream/gnuplot_i.hpp"

// Complie this using:
//  g++ -std=c++17 -o lex DFT.cpp -lboost_iostreams -lboost_system -lboost_filesystem

using std::cout;
using std::endl;
typedef std::vector <std::complex <double>> complexVector;
typedef std::complex <double> complexDouble;
void wait_for_key ();

complexVector dft ( complexVector array ) {
    double N = array.size();
    double F = N;

    complexVector output;
    output.reserve(N);

    double deltaF = 1;
    for ( double f = 0 ; f < F ; f+=deltaF ) {
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

    srand (1873257);

    double signalLength = 1000;
    complexVector signal;
    signal.reserve(signalLength);
    double signalFrequency = 6.30;
    double signalPhase = M_PI;

    std::vector <double> x;
    std::vector <double> y;


    for ( double i= 0 ; i < signalLength ; i++ ) {
        double noise_coeff = rand() % 10 + 1; // Inserting random pahse noise.
        double realP = cos ( ( ((2 * M_PI)/ signalLength) * (signalFrequency * static_cast<double>(i)) ) +  (signalPhase/noise_coeff)  );
        realP += cos ( ( ((2 * M_PI)/ signalLength) * ( (double)8.000 * static_cast<double>(i)) ) +  (signalPhase/noise_coeff)  );
        signal.push_back ( complexDouble ( realP, 0.000 ) );
        x.push_back(i);
        y.push_back(realP);
    }

    complexVector result = dft ( signal );

    std::vector <double> dft;

    double plotWidth  = 1000; // signalLength

    for ( double i = 0; i < plotWidth ; i++ ) {
        // freqDomain.push_back(  std::make_pair( i, std::abs(signal[i].real()))); //result[i]) ));
        dft.push_back( (std::abs( result[i] )/320) );
    }


        Gnuplot g1("lines");

        g1.set_grid();
        g1.set_style("lines").plot_xy(x,y,"Sginal");


        g1.set_style("lines").plot_xy(x,dft,"dft");

        g1.reset_plot();


        wait_for_key();

    return(0);


}

void wait_for_key ()
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__)  // every keypress registered, also arrow keys
    cout << endl << "Press any key to continue..." << endl;

    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
    _getch();
#elif defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
    cout << endl << "Press ENTER to continue..." << endl;

    std::cin.clear();
    std::cin.ignore(std::cin.rdbuf()->in_avail());
    std::cin.get();
#endif
    return;
}
