#include <vector>
#include <iostream>
#include <complex>
#include <math.h>
#include <map>
#include <cmath>
#include <stdlib.h>
#include <fstream>
#include <numeric>
#include <algorithm>





template < typename signalArraytype>
using  complexSignalVector_t  = std::vector <std::complex < signalArraytype >>;
template < typename signalArraytype>
using  signalVector_t   = std::complex < signalArraytype >;


template < typename signalArraytype>
class dsp {

private:
    double dummySignalFrequency; // =  atof( argv[1] );// Frequency of Signal
    
    bool enableDebugLog = false;

    complexSignalVector_t* signalPointer = NULL; // This is a pointer to the main signal, which is store on heap.
    
    int    sample_width;    // =  atof( argv[2] );// Number of Samples to average over
    double upperBound;      // =  atof( argv[3] );// 0.6500; // Upper bound Percentage change from average sample.
    double lowerBound;      // =  atof( argv[4] );// 0.6500; // Upper bound Percentage change from average sample.
    double avreagingStep;   // =  atof( argv[5] );// 5; // Number of samples to skip before taking a new average
    double scalingFactor;   // =  atof( argv[6] );// 1.0000; // Scale the filtered signal.
    double SginalVSft;      // =  atof( argv[7] );// 1.0000; // Vertical Shift the filtered signal.
    
    double omega;           // =  atof( argv[8] );// 0.9000; // Omega Value.
    
public:

    void LOG ( std::string message , short level );

    
    dsp( const signalVector_t&        signal );
    dsp( const complexSignalVector_t& signal );
    dsp( const signalArraytype*       signal, T length  );

    ~dsp (void)  {
        if (signalPointer != NULL) {
            delete signalPointer;
        }
    }

    doubleVector_t  noiseFilter ();
    complexVector_t dft ( complexVector_t signal );
    doubleVector_t  dft ( doubleVector_t signal );
    doubleVector_t dc_removal ();
    void wait_for_key ();
};


