#include "libinterface.h"

fftw_complex a;

bool li::fft(const std::vector<double> &signal, std::vector<double> &fReal, std::vector<double> &fImag)
{
    //TODO call library and return signals correct
    fReal=signal;
    fImag=signal;

}

bool li::ifft(const std::vector<double> &fReal,const std::vector<double> &fImag, std::vector<double> &signal)
{
    //TODO call library and return signals correct
    signal=fReal;


}

bool li::complex_pow(double realC, double imagC, double power)
{
    //TODO: fin lib and interface function.
}
