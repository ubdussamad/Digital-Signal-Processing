#include <iostream>
#include <vector>
#include <complex>
#include <math.h>
#include <map>
#include <cmath>
#include <stdlib.h>
#include <fstream>
#include "gnuplot-iostream/gnuplot_i.hpp"

// Complie this using:
//  g++ -std=c++17 -o lex DFT.cpp -lboost_iostreams -lboost_system -lboost_filesystem

using std::cout;
using std::endl;
typedef std::vector <std::complex <double>> complexVector;
typedef std::complex <double> complexDouble;
typedef std::vector <double> doubleVector;
void wait_for_key ();
doubleVector  noiseFilter (doubleVector y ,int sample_width,double upperBound,double lowerBound,double averagingStep,double scalingFactor, double verticalPhaseShift );
complexVector dft (  complexVector array );
doubleVector  dft ( doubleVector array );


int main ( int argc, char* argv[] ) {

    srand (1873257);
    // Args:  signalFrequency , sample_width, upper_bound, lower_bound, flag, scalingFactor, verticalPhaseShift
    
    bool readFile = false;
    std::string fileName;
    if ( (argc > 7) && (argc <11)) {
        fileName = argv[8];
        readFile = true;
    }

    else {
        cout << "Incorrect Usage of Arguments, follow: " << endl;
        cout << "Signal Frequency | Sample Width | Upper Bound | Lower Bound | Filter Width | Scaling Factor | Vertical Phase shift" << endl;
        cout << endl << "Example:" << endl;
        cout << "./a.out 4.00 50 0.4 0.4 5 1.00 2.00" << endl;
        return -1;
    }


    double signalFrequency =  atof( argv[1] ); // Frequency of Signal
    int    sample_width    =  atof( argv[2] ); // Number of Samples to average over
    double upperBound      =  atof( argv[3] ); //0.6500; // Upper bound Percentage change from average sample.
    double lowerBound      =  atof( argv[4] );//0.6500; // Upper bound Percentage change from average sample.
    double avreagingStep   =  atof( argv[5] );//5; // Number of samples to skip before taking a new average
    double scalingFactor   =  atof( argv[6] );//1.0000; // Scale the filtered signal.
    double SginalVSft      =  atof( argv[7] );//1.0000; // Vertical Shift the filtered signal.

    
    std::string myText;
    doubleVector siGx;
    doubleVector siGy;
    double counter = 0;
    if ( readFile ) {
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




    double signalLength = 1000;
    complexVector signal;
    signal.reserve(signalLength);
    double signalPhase = M_PI;

    doubleVector x;
    doubleVector y;
    doubleVector ref;


    // for ( double i= 0 ; i < signalLength ; i++ ) {
    //     double noise_coeff = rand() % 20 + 1; // Inserting random pahse noise.
    //     double realP = sin ( ( ((2 * M_PI)/ signalLength) * (signalFrequency * static_cast<double>(i)) ) +  (signalPhase/noise_coeff)  );
    //      realP += cos ( ( ((2 * M_PI)/ signalLength) * ( (double)8.000 * static_cast<double>(i)) ) +  (signalPhase/noise_coeff)  );
    //     signal.push_back ( complexDouble ( realP, 0.000 ) );
    //     ref.push_back( sin ( ( ((2 * M_PI)/ signalLength) * (signalFrequency * static_cast<double>(i)) ) + 0.000) + 2.00 +  cos ( ( ((2 * M_PI)/ signalLength) * ( (double)8.000 * static_cast<double>(i)) ) +  (signalPhase)  ));
    //     x.push_back(i);
    //     y.push_back(realP);
    // }

    doubleVector z = noiseFilter ( siGy,  sample_width, upperBound,
                             lowerBound, avreagingStep, scalingFactor , SginalVSft );

    doubleVector frequencyDomain = dft ( z );
    frequencyDomain = noiseFilter ( frequencyDomain,  sample_width, upperBound,
                             lowerBound, avreagingStep, scalingFactor , SginalVSft );
    

    Gnuplot g1("lines");
    g1.set_grid();
    g1.set_style("lines").plot_xy(siGx, siGy,            "Noisey Sginal");
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

        if ( (ctr == averagingStep) & (t+sample_width <= signalLength) ){
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


doubleVector dft ( doubleVector array ) {
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

        output.push_back( std::abs (tmp) * 0.0001 );
    }

    return ( output );
}