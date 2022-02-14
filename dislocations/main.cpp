#include <iostream>
#include <random>
#include <chrono>

using namespace std;

const int CS = 100;

default_random_engine generator(
        chrono::duration_cast<chrono::nanoseconds>(chrono::system_clock::now().time_since_epoch()).count()
);
uniform_real_distribution<double> distrib(0., 1.);

void generate_dis(unsigned char crystal[CS][CS], int n, int dis_x[CS], int dis_y[CS], int r) {
    for (int k = 0; k < r; k++) {
        dis_x[k] = (int) (distrib(generator) * n);
        dis_y[k] = (int) (distrib(generator) * n);
        crystal[dis_x[k]][dis_y[k]] = 1;
    }
}

void init(unsigned char crystal[CS][CS], int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            crystal[i][j] = 0;
        }
    }
}

void log(unsigned char crystal[CS][CS], int n) {
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

unsigned char dis_type(unsigned char crystal[CS][CS], int dis_x[CS], int dis_y[CS], int k) {
    return crystal[dis_x[k]][dis_y[k]];
}

bool check_alive(unsigned char crystal[CS][CS], int n, int dis_x[CS], int dis_y[CS], int r) {
    bool alive = false;
    for (int k = 0; k < r && !alive; k++) {
        if (dis_x[k] == 0 || dis_x[k] == n - 1 || dis_y[k] == 0 || dis_y[k] == n - 1) {
            crystal[dis_x[k]][dis_y[k]] = 2;
        }
        if (crystal[dis_x[k]][dis_y[k]]) {
            alive = true;
        }
    }
    return alive;
}

//int simulate(unsigned char crystal[CS][CS], int n, int dis_x[CS], int dis_y[CS], int r) {
//    for (int t = 0; check_alive(crystal, n, dis_x, dis_y, r); t++) {
//
//    }
//}

int main() {
    unsigned char crystal[CS][CS];
    int crystal_size = 6;
    init(crystal, crystal_size);

    int dis_x[CS], dis_y[CS], num_dis = 1;
    generate_dis(crystal, crystal_size, dis_x, dis_y, num_dis);
    check_alive(crystal, crystal_size, dis_x, dis_y, num_dis);

    log(crystal, crystal_size);
    return 0;
}


