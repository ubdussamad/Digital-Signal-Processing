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

typedef std::vector <double> doubleVector;
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

int main ( int argc, char* argv[] ) {

    srand (1873257);
    // signalFrequency , sample_width, upper_bound, lower_bound, flag, scaling_factor
    if (argc != 7) {
        cout << "Incorrect Usage: " << endl;
        cout << "Signal Frequency | Sample Width | Upper Bound | Lower Bound | Filter Width | Scaling Factor" << endl;
        cout << endl << "Example:" << endl;
        cout << "./a.out 4.00 50 0.4 0.4 5 1.00" << endl;
        return -1;
    }

    double signalFrequency = atof( argv[1] );
    int sample_width =       atof( argv[2] );
    double upperBound =      atof( argv[3] ); //0.6500; // Upper bound Percentage change from average sample.
    double lowerBound =      atof( argv[4] );//0.6500; // Upper bound Percentage change from average sample.
    double flag =            atof( argv[5] );//5;
    double scaling_factor =  atof( argv[6] );//1.0000;
    
    double ctr = flag;
    double local_mean = 0;



    double signalLength = 1000;
    complexVector signal;
    signal.reserve(signalLength);
    
    double signalPhase = M_PI;

    doubleVector x;
    doubleVector y;
    doubleVector ref;


    for ( double i= 0 ; i < signalLength ; i++ ) {
        double noise_coeff = rand() % 20 + 1; // Inserting random pahse noise.
        double realP = sin ( ( ((2 * M_PI)/ signalLength) * (signalFrequency * static_cast<double>(i)) ) +  (signalPhase/noise_coeff)  );
        // realP += cos ( ( ((2 * M_PI)/ signalLength) * ( (double)8.000 * static_cast<double>(i)) ) +  (signalPhase/noise_coeff)  );
        signal.push_back ( complexDouble ( realP, 0.000 ) );

        ref.push_back( sin ( ( ((2 * M_PI)/ signalLength) * (signalFrequency * static_cast<double>(i)) ) + 0.000) + 2.00);
        x.push_back(i);
        y.push_back(realP);
    }
    
    // DC bias filtering.
    // double α = 0.355;

    doubleVector z;
    doubleVector w;
    
    // z[0] = 0;
    // w[0] = 0;

    // for ( int t = 1; t < signalLength ; t++ ) {
    //     w[t] = x[t] + α * w[t-1];
        
    //     z[t] = w[t] - w[t-1]; 
    // }


    // z[0] = 0;
    // w[0] = 0;

    for ( int t = 0; t < signalLength ; t++ ) {

        if ( (ctr == flag) & (t+sample_width <= signalLength) ){
            double avg = 0;
            for ( double j = t; (j < (t+sample_width))   ; j++ ) { avg += y[j]; }
            local_mean = avg / sample_width;
            ctr=0;
        }
        bool xf = y[t] > (upperBound * local_mean);
        bool yf = y[t] < (lowerBound * local_mean);
        if ( xf ) { z.push_back( (upperBound * local_mean * scaling_factor) + 2.0000 ); }
        else if ( yf ) { z.push_back(  (lowerBound * local_mean * scaling_factor) + 2.0000  ); }
        else { z.push_back( (y[t] * scaling_factor    )+ 2.00000); }
        ctr++;
    }

 

    double plotWidth  = 1000; // signalLength


        Gnuplot g1("lines");

        g1.set_grid();
        g1.set_style("lines").plot_xy(x,y,"Noisey Sginal");


        g1.set_style("lines").plot_xy(x,z,"Filtered Signal");

        g1.set_style("lines").plot_xy(x,ref,"Original Signal");

        g1.reset_plot();


        wait_for_key();

    return(0);


}

void wait_for_key ()
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__)  // every keypress registered, also arrow keys
    //////cout << endl << "Press any key to continue..." << endl;

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
