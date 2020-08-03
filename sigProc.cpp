#include <iostream>
#include <vector>
#include <complex>
#include <math.h>
#include <map>
#include <cmath>
#include <stdlib.h>
#include <fstream>
#include <numeric>
#include <algorithm>
#include "gnuplot-iostream/gnuplot_i.hpp"

// Complie this using:
//  g++ -std=c++17 -o lex DFT.cpp -lboost_iostreams -lboost_system -lboost_filesystem

// ./lex 4 30 0.15 0.15 30 36.500 350 0.85 txt // Almost square wave heart rate output

using std::cout;
using std::endl;
typedef std::vector <std::complex <double>> complexVector;
typedef std::complex <double> complexDouble;
typedef std::vector <double> doubleVector;
void wait_for_key ();
doubleVector  noiseFilter (doubleVector y ,int sample_width,double upperBound,double lowerBound,double averagingStep,double scalingFactor, double verticalPhaseShift );
complexVector dft (  complexVector array , double dft_scaling_factor = 0.001 );
doubleVector  dft ( doubleVector array  , double dft_scaling_factor  );
doubleVector dc_removal ( doubleVector array , double alpha ,  double omegaI);


int main ( int argc, char* argv[] ) {

    srand (1873257);
    // Args:  signalFrequency , sample_width, upper_bound, lower_bound, flag, scalingFactor, verticalPhaseShift
    
    bool readFile = false;
    std::string fileName;
    if ( (argc > 8) && (argc <12)) {
        fileName = argv[9];
        readFile = true;
    }

    else {
        cout << "Incorrect Usage of Arguments, follow: " << endl;
        cout << "Signal Frequency | Sample Width | Upper Bound | Lower Bound | Filter Width | Scaling Factor | Vertical Phase shift" << endl;
        cout << endl << "Example:" << endl;
        cout << "./a.out 4.00 50 0.4 0.4 5 1.00 2.00" << endl;
        return -1;
    }


    double signalFrequency =  atof( argv[1] );// Frequency of Signal
    int    sample_width    =  atof( argv[2] );// Number of Samples to average over
    double upperBound      =  atof( argv[3] );// 0.6500; // Upper bound Percentage change from average sample.
    double lowerBound      =  atof( argv[4] );// 0.6500; // Upper bound Percentage change from average sample.
    double avreagingStep   =  atof( argv[5] );// 5; // Number of samples to skip before taking a new average
    double scalingFactor   =  atof( argv[6] );// 1.0000; // Scale the filtered signal.
    double SginalVSft      =  atof( argv[7] );// 1.0000; // Vertical Shift the filtered signal.
    double omega           =  atof( argv[8] );// 0.9000; // Omega Value.

    
    std::string myText;
    doubleVector siGx;
    doubleVector siGy;
    double counter = 0;

    if ( readFile ) {
    cout << "Reading Signal data from file." << endl;
    std::ifstream MyReadFile(fileName);
    while (getline (MyReadFile, myText)) {
        double i = atof( myText.c_str() );
        if (i>0) {
            siGy.push_back(i);
            siGx.push_back(counter);
            counter++;
            }
        }
    MyReadFile.close(); 
    }
    else { 
        cout << "Genrating Random signal internally to test the filter." <<  endl;
    }


    double signalLength = 1000;
    complexVector signal;
    signal.reserve(signalLength);
    double signalPhase = M_PI;

    doubleVector x;
    doubleVector y;
    doubleVector ref;

    // Signal Genrator
    // for ( double i= 0 ; i < signalLength ; i++ ) {
    //     double noise_coeff = rand() % 20 + 1; // Inserting random pahse noise.
    //     double realP = sin ( ( ((2 * M_PI)/ signalLength) * (signalFrequency * static_cast<double>(i)) ) +  (signalPhase/noise_coeff)  );
    //      realP += cos ( ( ((2 * M_PI)/ signalLength) * ( (double)8.000 * static_cast<double>(i)) ) +  (signalPhase/noise_coeff)  );
    //     signal.push_back ( complexDouble ( realP, 0.000 ) );
    //     ref.push_back( sin ( ( ((2 * M_PI)/ signalLength) * (signalFrequency * static_cast<double>(i)) ) + 0.000) + 2.00 +  cos ( ( ((2 * M_PI)/ signalLength) * ( (double)8.000 * static_cast<double>(i)) ) +  (signalPhase)  ));
    //     x.push_back(i);
    //     y.push_back(realP);
    // }

    doubleVector z = dc_removal ( siGy , omega , 0.0);

    cout << "Length of z before DC removal: " <<  z.size() << endl;


    z = noiseFilter ( z,  sample_width, upperBound,
                             lowerBound, avreagingStep, scalingFactor , SginalVSft );

    cout << "Length of z before DC removal: " <<  z.size() << endl;

    doubleVector frequencyDomain = dft ( z , 0.01);
    // frequencyDomain = noiseFilter ( frequencyDomain,  sample_width, upperBound,
    //                          lowerBound, avreagingStep, scalingFactor , SginalVSft );
    
    cout << "Frequency: " <<  std::max_element( frequencyDomain.begin() , frequencyDomain.end() ) - frequencyDomain.begin() << "Hz" << endl;

    Gnuplot g1("lines");
    g1.set_grid();
    g1.set_style("lines").plot_xy(siGx, siGy,         "Noisey Sginal");
    g1.set_style("lines").plot_xy(siGx, z,               "Filtered Signal");
    g1.set_style("lines").plot_xy(siGx, frequencyDomain, "DFT");
    g1.reset_plot();
    wait_for_key();

    return(0);


}


void wait_for_key () {
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


doubleVector noiseFilter ( doubleVector y, int sample_width,double upperBound,double lowerBound,double averagingStep,double scalingFactor , double verticalPhaseShift ) {

    doubleVector z;
    int signalLength = y.size();
    double ctr = averagingStep;
    double local_mean = 0;


    for ( int t = 0; t < signalLength ; t++ ) {

        if ( (ctr == averagingStep) & (t+sample_width <= signalLength) ) {
            double avg = 0;
            for ( double j = t; (j < (t+sample_width))   ; j++ ) { avg += y[j]; }
            local_mean = avg / sample_width;
            ctr=0;
        }
        bool xf = y[t] > (upperBound * local_mean);
        bool yf = y[t] < (lowerBound * local_mean);
        if ( xf ) { z.push_back( (upperBound * local_mean * scalingFactor) + verticalPhaseShift ); }
        else if ( yf ) { z.push_back(  (lowerBound * local_mean * scalingFactor) + verticalPhaseShift  ); }
        else { z.push_back( (y[t] * scalingFactor    )+ verticalPhaseShift ); }
        ctr++;
    }

    return ( z );
}

doubleVector dc_removal ( doubleVector array , double alpha ,  double omegaI ) {
    doubleVector omega;
    omega.push_back ( omegaI );
    for ( unsigned int i = 0x0 ; i < array.size() ; i++ ) {
        omega.push_back ( array[i] + alpha * (omega.back()) );
    }

    doubleVector reduX;
    reduX.push_back(omegaI);
    for ( unsigned int i = 0x1 ; i < array.size() ; i++ ) {
        reduX.push_back ( omega[i] - omega[i-1] );
    }



    return reduX;
}

doubleVector dft ( doubleVector array , double dft_scaling_factor ) {
    double N = array.size();
    double F = N;

    doubleVector output;
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

        output.push_back( std::abs (tmp) * dft_scaling_factor );
    }

    return ( output );
}
