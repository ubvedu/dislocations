#include <iostream>
#include <random>
#include <chrono>

using namespace std;

const int MCS = 100;

void single(unsigned char crystal[MCS][MCS], int n) {

    auto time = chrono::system_clock::now().time_since_epoch();
    default_random_engine engine(chrono::duration_cast<chrono::nanoseconds>(time).count());
    uniform_real_distribution<double> distrib(0., 1.);

    crystal[(int)(distrib(engine) * n)][(int)(distrib(engine) * n)] = 1;
}

void init(unsigned char crystal[MCS][MCS], int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            crystal[i][j] = 0;
        }
    }
}

void log(unsigned char crystal[MCS][MCS], int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            switch (crystal[i][j]) {
                case 1:
                    cout << "o";
                    break;
                case 2:
                    cout << "x";
                    break;
                default:
                    cout << ".";
            }
            cout << " ";
        }
        cout << endl;
    }
}

int main() {
    unsigned char crystal[MCS][MCS];
    int n = 6;
    init(crystal, n);
    single(crystal, n);
    log(crystal, n);
    return 0;
}


