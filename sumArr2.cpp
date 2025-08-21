#include <omp.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

int main() {
    const int N = 100000000;
    vector<int> arr(N, 1);
    long long sum = 0;

    srand(time(NULL));
    for (int i = 0; i < N; i++) arr[i] = rand() % 10;

    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        int total = omp_get_num_threads();
        #pragma omp critical
        cout << "Hilo " << tid << " de " << total << " hilos creados" << endl;
        #pragma omp for reduction(+:sum)
        for (int i = 0; i < N; i++) {
            sum += arr[i];
        }
    }

    cout << "Suma total (OpenMP) = " << sum << endl;
    return 0;
}