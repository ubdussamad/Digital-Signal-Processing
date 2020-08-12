#include "SignalProcessingUtil.hpp"
#include "gnuplot-iostream/gnuplot_i.hpp"


template < typename signalArraytype>
void dsp<signalArraytype>::LOG ( std::string message , short level ) {
    if (level == 1 || enableDebugLog_ ) {
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
complexSignalVector_t<signalArraytype> dsp <signalArraytype> :: dft ( const signalArraytype sweepDelta,const double scaling_factor ) {

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
            tmp += inputSignal[n] * z;
        }

        output.push_back( tmp * scaling_factor );
    }

    return ( output );
}


int main (int argc, char* argv[]) {
    srand (std::time(0));
    int signalLength = atoi(argv[1]);
    double signalFrequency = 80.00;
    double signalPhase = M_PI/4;
    complexSignalVector_t<double> signal;
    // Signal Genrator

    std::vector <double> siGx , siGy;
    for ( double i= 0 ; i < signalLength ; i++ ) {
        siGx.push_back(i);
        double noise_coeff = rand() % 20 + 1; // Inserting random phase noise.
        double realP = sin ( ( ((2 * M_PI)/ signalLength) * (signalFrequency * static_cast<double>(i)) ) +  (signalPhase/noise_coeff)  );
        realP += cos ( ( ((2 * M_PI)/ signalLength) * ( (double)40.000 * static_cast<double>(i)) ) +  (signalPhase/noise_coeff)  );
        signal.push_back ( std::complex <double> ( realP, 0.000 ) );
        siGy.push_back(realP);

    }

    std::cout << "Workin this block!" << std::endl;
    dsp<double>::enableDebugLog(true);
    dsp<double> x( signal );

    auto X = x.dft( 0.05 , 0.1);

    std::vector <double> z;

    for ( double i= 0 ; i < signalLength ; i++ ) {
        z.push_back(std::abs(X[i]));
    }



    Gnuplot g1("lines");
    g1.set_grid();
    g1.set_style("lines").plot_xy(siGx, siGy,         "Noisey Sginal");
    // g1.set_style("lines").plot_xy(siGx, z,               "Filtered Signal");
    g1.set_style("lines").plot_xy(siGx, z, "DFT");
    g1.reset_plot();
    wait_for_key();
    


    return(0);
}