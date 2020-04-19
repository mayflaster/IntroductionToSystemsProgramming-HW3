#ifndef EX3_COMPLEX_H
#define EX3_COMPLEX_H
#include <math.h>
#include <iostream>

namespace MtmMath {
    class Complex {
        double re,im;
    public:
        Complex(double r=0, double i=0) : re(r) ,im(i) {};
        Complex(const Complex&)= default;
        Complex& operator=(const Complex&)= default ;
        Complex operator-() const{
            return Complex(-re, -im);
        }
        Complex& operator+=(const Complex& c){
            re += c.re;
            im += c.im;
            return *this;
        }
        Complex& operator-=(const Complex& c){
            return *this+= -c;
        }
        Complex& operator *=(const Complex& c){
            double temp =  re*c.re - im*c.im;
            im = im*c.re + re*c.im;
            re = temp;
            return *this;
        }
        friend bool operator==(const Complex& a, const Complex& b){
            return a.re == b.re && a.im == b.im;
        }
        friend std::ostream& operator<<(std::ostream& os, const Complex& c){
            const char* sign = c.im < 0 ? "-" : "+";
            return os << c.re << sign << c.im << "i";
        }
        friend std::istream& operator>>(std::istream& is, Complex& c){
            return is >> c.re >> c.im;
        }
        const double getReal(){
            return re;
        }

        const double getIm(){
            return im;
        }

    };
    Complex operator+(const Complex& a, const Complex& b){
        return Complex(a) += b;
    }
    Complex operator-(const Complex& a, const Complex& b){
        return Complex(a) -=b ;
    }

    Complex operator*(const Complex& a, const Complex& b){
        return Complex(a) *= b;
    }

    bool operator!=(const Complex& a, const Complex& b) {
        return !(a == b);
    }
}
#endif //EX3_COMPLEX_H
