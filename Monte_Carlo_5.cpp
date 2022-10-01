#include <iostream>
#include <cmath>
#include <iomanip>

using namespace std;

typedef long double(*FuncPtr)(long double);

long double func(long double x);

long double definite_integral(FuncPtr func_ptr, double min, double max, short div_log2);

int main() {
    cout << setprecision(20) << definite_integral(func, 0, 1, -25) << endl;
}

long double func(long double x) {
    return exp(-pow(x, 3));
}

long double definite_integral(FuncPtr func_ptr, double min, double max, short div_log2) {
    auto div = pow(2, div_log2);
    if (fmod(max - min, div) != 0.0) {
        cerr << "Invalid number." << endl;
    } else if (fmod(min, div) != 0.0) {
        cerr << "Invalid min number." << endl;
    } else if (fmod(max, div) != 0.0) {
        cerr << "Invalid max number." << endl;
    } else if (div_log2 > -4) {
        cerr << "Invalid div_log2 number.\nPlease set -5 or smaller." << endl;
    }
    long double sum = 0;
    //long double all_sum = 0;
    //unsigned short min_width = 0;
    for (int i = static_cast<int>(min / div); i < static_cast<int>(max / div); ++i) {
        sum += func_ptr(i * div);
        //if (min_width == 32) {
        //    all_sum += sum;
        //    sum = 0;
        //    min_width = 0;
        //}
        //min_width++;
        //sum += func(i * div);
    }
    //return all_sum * div;
    return sum * div;

}