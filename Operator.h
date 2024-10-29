#include <vector>
#include <cmath>
#include "iostream"
#include <iomanip>
#include <stdio.h>
#include <string.h>
using namespace std;
namespace final{
    typedef vector<double> Vec;
    typedef vector<Vec> Matrix;
    typedef vector<Matrix> BigMatrix;
    
    Vec operator-(const Vec& X, const Vec& Y);
    Vec operator*(const Vec& X, const Vec& Y);
    Vec& operator+=(Vec & X, const Vec & Y);
    Vec& operator-=(Vec & X, const Vec & Y);
    
    std::ostream& operator<<(std::ostream &out,Vec &X);
    std::ostream& operator<<(std::ostream &out,Matrix &Y);
    std::ostream& operator<<(std::ostream &out,BigMatrix &Z);
    std::ostream& operator<<(std::ostream &out,vector<string> &U);
}