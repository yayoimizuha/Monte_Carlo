#pragma clang diagnostic push
#pragma ide diagnostic ignored "openmp-use-default-none"

#include <iostream>
#include <array>
#include <random>
#include <iomanip>
#include <omp.h>

using namespace std;

size_t SIZE = 2;
constexpr size_t DIMENSION = 3;
#define GNUPLOT_PATH "C:\\Progra~1\\gnuplot\\bin\\gnuplot.exe -persist"

random_device seed_gen;
mt19937 engine(seed_gen());
uniform_real_distribution<double> distribution(-1.0, 1.0);

bool check(array<double, DIMENSION> arg);

int main() {

    int loop;
    cin >> loop;

    FILE *gnuplot;
    if ((gnuplot = popen(GNUPLOT_PATH, "w")) == nullptr) {
        cerr << "Can't execute gnuplot. " << GNUPLOT_PATH << endl;
        return EXIT_FAILURE;
    }
    fprintf(gnuplot, "set terminal windows color\n");
    fprintf(gnuplot, "set grid\n");
    fprintf(gnuplot, "set logscale x\n");
    fprintf(gnuplot, "set auto fix\n");
    fprintf(gnuplot, "%s\n", ("f1(x)=" + to_string(numbers::pi)).c_str());
    fprintf(gnuplot, "plot [0.3:%f] [-0.5:5] f1(x) lw 2 lc 'red' title 'baseline',"
                     "'-' using 1:2 w l lw 5 lc 'green'\n\n", pow(2, loop + 1));

    omp_set_num_threads(omp_get_max_threads());
    for (int i = 0; i < loop; ++i) {
        vector<size_t> count(omp_get_max_threads(), 0);
#pragma omp parallel for
        for (size_t j = 0; j < SIZE; ++j) {
            if (check({distribution(engine), distribution(engine), distribution(engine)})) {
                count[omp_get_thread_num()]++;
            }
        }
        double ans =
                static_cast<double>(accumulate(count.begin(), count.end(), static_cast<size_t>(0)) * 6)
                / static_cast<double>(SIZE);
        cout << fixed << SIZE << ' ' << setprecision(10) << ans << endl;
        //for (size_t x: count)cout << x << endl;
        //cout << "\n\n\n\n\n" << endl;
        fprintf(gnuplot, "%zu %lf\n", SIZE, ans);
        SIZE *= 2;
    }
    fprintf(gnuplot, "e\n");
    pclose(gnuplot);
}

bool check(array<double, DIMENSION> arg) {
    if (pow(arg[0], 2) + pow(arg[1], 2) + pow(arg[2], 2) < 1) {
        return true;
    } else return false;
}

#pragma clang diagnostic pop