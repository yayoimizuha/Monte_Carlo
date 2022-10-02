#pragma clang diagnostic push
#pragma ide diagnostic ignored "openmp-use-default-none"

#include <iostream>
#include <array>
#include <random>
#include <iomanip>
#include <omp.h>
#include <numeric>

using namespace std;

constexpr size_t DIMENSION = 2;
#define GNUPLOT_PATH "C:\\Progra~1\\gnuplot\\bin\\gnuplot.exe -persist"

random_device seed_gen;
mt19937 engine(seed_gen());

bool check(array<double, DIMENSION> arg);

int main() {

    FILE *gnuplot;
    if ((gnuplot = _popen(GNUPLOT_PATH, "w")) == nullptr) {
        cerr << "Can't execute gnuplot. " << GNUPLOT_PATH << endl;
        return EXIT_FAILURE;
    }
    fprintf(gnuplot, "set terminal windows color\n");
    fprintf(gnuplot, "set grid\n");
    fprintf(gnuplot, "set size ratio -1\n");
    //fprintf(gnuplot, "set size square\n");
    fprintf(gnuplot, "plot '-' using 1:2 w p title 'true','-' using 1:2 w p title 'false'\n\n");

    constexpr size_t log_size = 28;
    const auto SIZE = static_cast<size_t>(pow(2, log_size));

    uniform_real_distribution<double> distribution_x(0, 1);
    uniform_real_distribution<double> distribution_y(0, 1.2);

    string true_str, false_str;

    omp_lock_t ompLock;
    omp_init_lock(&ompLock);
    omp_set_num_threads(omp_get_max_threads());
    vector<size_t> count(omp_get_max_threads(), 0);
#pragma omp parallel for
    for (size_t j = 0; j < SIZE; ++j) {
        auto x = distribution_x(engine);
        auto y = distribution_y(engine);
        if (check({x, y})) {
            count[omp_get_thread_num()]++;
            if (j % 5000 == 0) {
                omp_set_lock(&ompLock);
                true_str += to_string(x) + " " + to_string(y) + "\n";
                omp_unset_lock(&ompLock);
            }
        } else if (j % 5000 == 0) {
            omp_set_lock(&ompLock);
            false_str += to_string(x) + " " + to_string(y) + "\n";
            omp_unset_lock(&ompLock);
        }
    }
    auto true_sum = accumulate(count.begin(), count.end(), static_cast<size_t>(0));
    auto true_ratio = static_cast<long double>(true_sum) / static_cast<long double>(SIZE);
    auto ans = true_ratio * (1 - 0) * (1.2 - 0);
    cout << fixed << setprecision(10) << ans << endl;


    fprintf(gnuplot, "%s", true_str.c_str());
    fprintf(gnuplot, "e\n");

    fprintf(gnuplot, "%s", false_str.c_str());
    fprintf(gnuplot, "e\n");

    pclose(gnuplot);

    //for (size_t x: count)cout << x << endl;
    //cout << "\n\n\n\n\n" << endl;
}

bool check(array<double, DIMENSION> arg) {
    if (exp(-pow(arg[0], 3)) > arg[1]) {
        return true;
    } else return false;
}

#pragma clang diagnostic pop