#include <iostream>
#include <random>
#include <chrono>
#include <fstream>

using namespace std;

const int CS = 1000;

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

void print(unsigned char crystal[CS][CS], int n) {
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
    cout << endl;
}

bool check_alive(unsigned char crystal[CS][CS], int n, int dis_x[CS], int dis_y[CS], int r) {
    bool alive = false;
    for (int k = 0; k < r && !alive; k++) {
        auto x = dis_x[k];
        auto y = dis_y[k];
        if (x == 0 || x == n - 1 || y == 0 || y == n - 1) {
            crystal[x][y] = 2;
        }
        // TODO: Add sticking for multiple dislocations
        if (crystal[x][y] == 1) {
            alive = true;
        }
    }
    return alive;
}

void move(unsigned char crystal[CS][CS], int n, int dis_x[CS], int dis_y[CS], int r) {
    for (int k = 0; k < r; k++) {
        auto x = dis_x[k];
        auto y = dis_y[k];
        if (crystal[x][y] == 2) {
            continue;
        }
        crystal[x][y] = 0;

        switch ((int) (distrib(generator) * 4)) {
            case 0:
                y--;
                break;
            case 1:
                x++;
                break;
            case 2:
                y++;
                break;
            case 3:
                x--;
                break;
        }
        dis_x[k] = x;
        dis_y[k] = y;
        crystal[x][y] = 1;
        // TODO: Add crossing checking
        // TODO: Add lazy moving
    }
}

int simulate(int crystal_size, int num_dislocations) {
    unsigned char crystal[CS][CS];
    init(crystal, crystal_size);

    int dis_x[CS], dis_y[CS];
    generate_dis(crystal, crystal_size, dis_x, dis_y, num_dislocations);

    int t;
    for (t = 0; check_alive(crystal, crystal_size, dis_x, dis_y, num_dislocations); t++) {
        move(crystal, crystal_size, dis_x, dis_y, num_dislocations);
    }

    return t;
}

double simulate_many(int crystal_size, int num_dislocations, int times) {
    int sum_steps = 0;
    for (int i = 0; i < times; i++) {
        sum_steps += simulate(crystal_size, num_dislocations);
    }
    return ((double) sum_steps) / times;
}

int main() {
    int times = 100;
    ofstream file("../out/single.csv");
    if (file.is_open()) {
        file << "crystal size,mean steps" << endl;

        for (int n = 1; n <= 1000; n++) {
            auto start = chrono::system_clock::now();

            auto mean_steps = simulate_many(n, 1, times);

            auto duration = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now() - start);
            cout << "Simulation with crystal size " << n << " ended in " << duration.count() << "ms" << endl;

            file << n << "," << mean_steps << endl;
        }
    } else {
        cout << "Unable to open file" << endl;
    }
    file.close();
    return 0;
}


