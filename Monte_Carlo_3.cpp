#include <iostream>
#include <array>
#include <random>

using namespace std;

#define GNUPLOT_PATH "C:\\Progra~1\\gnuplot\\bin\\gnuplot.exe -persist"
constexpr size_t SIZE = 32e+4;
constexpr size_t BUF = 256;
constexpr size_t DIMENSION = 3;
static_assert((SIZE % BUF) == 0, "Please set SIZE & BUF are able to integer multiple");

random_device seed_gen;
mt19937 engine(seed_gen());
uniform_real_distribution<double> distribution(-1.0, 1.0);

bool check(array<double, DIMENSION> arg);

int main() {

    FILE *gnuplot;
    if ((gnuplot = popen(GNUPLOT_PATH, "w")) == nullptr) {
        cerr << "Can't execute gnuplot. " << GNUPLOT_PATH << endl;
        return EXIT_FAILURE;
    }
    fprintf(gnuplot, "set terminal windows color\n");
    fprintf(gnuplot, "set grid\n");
    fprintf(gnuplot, "set ticslevel 0\n");
    fprintf(gnuplot, "set view equal xyz\n");
    fprintf(gnuplot,
            "splot [-1:1][-1:1][-1:1] '-' using 1:2:3 w p ps 1 pt 7 lc rgb '#AAFF0000' title 'true','-' using 1:2:3 w p ps 1 pt 7 lc rgb '#EA00FF00' title 'false'\n\n");

    array<double, DIMENSION> coordinate{};
    string str_true, str_false;
    size_t count_true = 0, count_false = 0;
    for (int i = 0; i < SIZE / BUF; ++i) {
        array<pair<bool, array<double, DIMENSION>>, BUF> buffer{};
        for (int j = 0; j < BUF; ++j) {
            for (double &x: coordinate) {
                x = distribution(engine);
            }
            buffer[j] = make_pair(check(coordinate), coordinate);
        }
        for (int j = 0; j < BUF; ++j) {
            if (buffer[j].first) {
                for (double x: buffer[j].second) {
                    str_true += to_string(x) + ' ';
                }
                str_true += '\n';
                count_true++;
            } else {
                for (double x: buffer[j].second) {
                    str_false += to_string(x) + ' ';
                }
                str_false += '\n';
                count_false++;
            }
        }
        str_true += '\n';
        str_false += '\n';
    }
    cout << (static_cast<float>(count_true) / SIZE) * 6 << endl;

    fprintf(gnuplot, "%s", str_true.c_str());
    fprintf(gnuplot, "e\n");

    fprintf(gnuplot, "%s", str_false.c_str());
    fprintf(gnuplot, "e\n");

    pclose(gnuplot);
}

bool check(array<double, DIMENSION> arg) {
    if (pow(arg[0], 2) + pow(arg[1], 2) + pow(arg[2], 2) < 1) {
        return true;
    } else return false;
}