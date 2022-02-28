#include <iostream>
#include <random>
#include <chrono>
#include <fstream>

using namespace std;

const int N = 1000;

//long long seed = 1645424332869499201;
long long seed = chrono::duration_cast<chrono::nanoseconds>(chrono::system_clock::now().time_since_epoch()).count();
default_random_engine generator(seed);
uniform_real_distribution<double> distrib(0., 1.);

void generate_dis(
        unsigned char crystal[N][N],
        unsigned short n,
        unsigned short dis_x[],
        unsigned short dis_y[],
        unsigned r
) {
    for (int k = 0; k < r; k++) {
        crystal[dis_x[k]][dis_y[k]] = 1;
    }
}

void print(unsigned char crystal[N][N], unsigned short m, unsigned short n) {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            switch (crystal[j][i]) {
                case 0:
                    cout << ".";
                    break;
                case 1:
                    cout << "o";
                    break;
                case 2:
                    cout << "x";
                    break;
                default:
                    cout << crystal[j][i];
            }
            cout << " ";
        }
        cout << endl;
    }
    cout << endl;
}

bool check_alive(
        unsigned char crystal[N][N],
        unsigned short n,
        unsigned short dis_x[],
        unsigned short dis_y[],
        unsigned r,
        bool linear
) {
    bool alive = false;
    for (int k = 0; k < r; k++) {
        auto x = dis_x[k];
        auto y = dis_y[k];
        if (crystal[x][y] == 2) {
            continue;
        }
        if (x == 0 || x == n - 1 || (y == 0 || y == n - 1) && !linear) {
            crystal[x][y] = 2;
        } else if (crystal[x + 1][y] || crystal[x][y + 1] || crystal[x - 1][y] || crystal[x][y - 1]) {
            crystal[x][y] = 2;
        }
        if (crystal[x][y] == 1) {
            alive = true;
        }
    }
    return alive;
}

void move(unsigned char crystal[N][N], unsigned short dis_x[], unsigned short dis_y[], unsigned r, bool linear) {
    for (int k = 0; k < r; k++) {
        auto x = dis_x[k];
        auto y = dis_y[k];
        if (crystal[x][y] == 2) {
            continue;
        }
        crystal[x][y] = 0;
        int dirs;
        if (linear) {
            dirs = 2;
        } else {
            dirs = 4;
        }
        switch ((int) (distrib(generator) * dirs)) {
            case 0:
                x++;
                break;
            case 1:
                x--;
                break;
            case 2:
                y--;
                break;
            case 3:
                y++;
                break;
        }
        if (!crystal[x][y]) {
            dis_x[k] = x;
            dis_y[k] = y;
            crystal[x][y] = 1;
        } else {
            crystal[dis_x[k]][dis_y[k]] = 1;
        }
    }
}

void clear_crystal(unsigned char crystal[N][N], unsigned short n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            crystal[i][j] = 0;
        }
    }
}

int simulate(
        unsigned char crystal[N][N],
        unsigned short crystal_size,
        unsigned short dis_x[],
        unsigned short dis_y[],
        unsigned num_dislocations,
        bool linear
) {
    clear_crystal(crystal, crystal_size);
    generate_dis(crystal, crystal_size, dis_x, dis_y, num_dislocations);

    int t;
    for (t = 0; check_alive(crystal, crystal_size, dis_x, dis_y, num_dislocations, linear); t++) {
        // print(crystal, 1, crystal_size);
        move(crystal, dis_x, dis_y, num_dislocations, linear);
    }

    // print(crystal, 1, crystal_size);

    return t;
}

void run_single() {
    int times = 100;
    ofstream file("../out/single.csv");
    if (file.is_open()) {
        file << "crystal size,mean steps" << endl;

        unsigned char crystal[N][N];
        unsigned short dis_x[1], dis_y[1];
        for (int n = 1; n <= N; n++) {
            auto start = chrono::system_clock::now();

            int sum_steps = 0;
            for (int i = 0; i < times; i++) {
                dis_x[0] = (unsigned short) (distrib(generator) * n);
                dis_y[0] = (unsigned short) (distrib(generator) * n);
                sum_steps += simulate(crystal, n, dis_x, dis_y, 1, false);
            }
            auto mean_steps = sum_steps / (double) times;

            auto duration = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now() - start);
            cout << "single run / crystal size: " << n << ", duration: " << duration.count() << "ms" << endl;

            file << n << "," << mean_steps << endl;
        }
    } else {
        cout << "Unable to open file" << endl;
    }
    file.close();
}

void init_x(unsigned  short dis_x[], unsigned short n) {
    for (int x = 0; x < n; x++) {
        dis_x[x] = x;
    }
}

void shuffle_x(unsigned short dis_x[], unsigned short n) {
    for (int i = 0; i < n; i++) {
        auto j = (int) (distrib(generator) * n);
        auto tmp = dis_x[i];
        dis_x[i] = dis_x[j];
        dis_x[j] = tmp;
    }
}

void init_xy(unsigned short dis_x[], unsigned short dis_y[], unsigned short n) {
    for (int x = 0; x < n; x++) {
        for (int y = 0; y < n; y++) {
            dis_x[x * n + y] = x;
            dis_y[x * n + y] = y;
        }
    }
}

void shuffle_xy(unsigned short dis_x[], unsigned short dis_y[], unsigned short n) {
    auto n2 = n * (int) n;
    for (int i = 0; i < n2; i++) {
        auto j = (int) (distrib(generator) * n);
        auto tmp = dis_x[i];
        dis_x[i] = dis_x[j];
        dis_x[j] = tmp;
        tmp = dis_y[i];
        dis_y[i] = dis_y[j];
        dis_y[j] = tmp;
    }
}

void run_multiple() {
    ofstream file("../out/multiple.csv");
    if (file.is_open()) {
        file << "crystal size,num dislocations,mean steps" << endl;

        unsigned char crystal[N][N];
        unsigned short dis_x[N * N], dis_y[N * N];
        for (int n = 2; n <= N; n++) {
            auto start = chrono::system_clock::now();

            init_xy(dis_x, dis_y, n);
            shuffle_xy(dis_x, dis_y, n);
            auto r = (int) (distrib(generator) * n * n);
            auto steps = simulate(crystal, n, dis_x, dis_y, r, false);
            file << n << "," << r << "," << steps << endl;

            auto end = chrono::system_clock::now();
            auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
            cout << "multiple run / crystal size: " << n << ", n dislocations: " << r << ", duration: "
                 << duration.count() << "ms" << endl;
        }
    } else {
        cout << "Unable to open file" << endl;
    }
    file.close();
}

int main() {
    cout << "random seed " << seed << endl;

    unsigned char crystal[N][N];
    unsigned short dis_x[N];
    unsigned short dis_y[N] = {0};
    auto n = 40;
    auto r = 5;
    init_x(dis_x, n);
    shuffle_x(dis_x, n);

    simulate(crystal, n, dis_x, dis_y, r, true);

    return 0;
}


