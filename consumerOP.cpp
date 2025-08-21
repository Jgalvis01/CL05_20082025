#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <omp.h>
#include <algorithm>

using namespace std;

const int N = 10;
vector<int> buffer(N, 0);
omp_lock_t lock_var;

void producer(int id) {
    while (true) {
        int val = rand() % 9 + 1;

        omp_set_lock(&lock_var);
        auto it = find(buffer.begin(), buffer.end(), 0);
        if (it != buffer.end()) {
            *it = val;
            cout << "Producer " << id << " produced " << val << endl;
        }
        omp_unset_lock(&lock_var);

        usleep((rand() % 500 + 100) * 1000); // espera aleatoria
    }
}

void consumer(int id) {
    while (true) {
        omp_set_lock(&lock_var);
        auto it = find_if(buffer.begin(), buffer.end(), [](int v){ return v != 0; });
        if (it != buffer.end()) {
            int val = *it;
            *it = 0;
            cout << "Consumer " << id << " consumed " << val << endl;
        }
        omp_unset_lock(&lock_var);

        usleep((rand() % 500 + 100) * 1000); // espera aleatoria
    }
}

int main() {
    srand(time(NULL));
    omp_init_lock(&lock_var);

    #pragma omp parallel sections
    {
        #pragma omp section
        producer(0);
        #pragma omp section
        producer(1);
        #pragma omp section
        consumer(0);
        #pragma omp section
        consumer(1);
    }

    omp_destroy_lock(&lock_var);

    cout << "Final buffer state: ";
    for (int v : buffer) cout << v << " ";
    cout << endl;
}
