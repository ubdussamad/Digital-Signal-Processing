#include <iostream>
#include <vector>
#include <complex>
#include <math.h>
#include <map>
#include <cmath>
#include "gnuplot-iostream/gnuplot-iostream.h"

// Complie this using:
//  g++ -std=c++17 -o lex DFT.cpp -lboost_iostreams -lboost_system -lboost_filesystem

typedef std::vector <std::complex <double>> complexVector;
typedef std::complex <double> complexDouble;

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

    // double signalLength = 1000;
    // complexVector signal;
    // signal.reserve(signalLength);
    // double signalFrequency = 6.30;
    // double signalPhase = M_PI / 4;

    // for ( double i= 0 ; i < signalLength ; i++ ) {
    //     double realP = cos ( ( ((2 * M_PI)/ signalLength) * (signalFrequency * static_cast<double>(i)) ) +  signalPhase  );
    //     signal.push_back ( complexDouble ( realP, 0.000 ) );
    // }

    // complexVector result = dft ( signal );

    // for ( double i = 0; i<70 ; i++ ) {
    //     std::cout << i/10 << ": "  << std::abs(result[i]) << std::endl;
    // }

    Gnuplot gp;

	std::vector<std::pair<double, double> > xy_pts_A;
	for(double x=-2; x<2; x+=0.01) {
		double y = x*x*x;
		xy_pts_A.push_back(std::make_pair(x, y));
	}

	std::vector<std::pair<double, double> > xy_pts_B;
	for(double alpha=0; alpha<1; alpha+=1.0/24.0) {
		double theta = alpha*2.0*3.14159;
		xy_pts_B.push_back(std::make_pair(cos(theta), sin(theta)));
	}

	gp << "set xrange [-2:2]\nset yrange [-2:2]\n";
	// Data will be sent via a temporary file.  These are erased when you call
	// gp.clearTmpfiles() or when gp goes out of scope.  If you pass a filename
	// (e.g. "gp.file1d(pts, 'mydata.dat')"), then the named file will be created
	// and won't be deleted (this is useful when creating a script).
	gp << "plot" << gp.file1d(xy_pts_A) << "with lines title 'cubic',"
		<< gp.file1d(xy_pts_B) << "with points title 'circle'" << std::endl;

    return(0);


}