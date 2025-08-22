#include <iostream>
#include <cstring>
#include <omp.h>
using namespace std;

void Count_sort(int a[], int n, int thread_count) {
    int* temp = new int[n];

    #pragma omp parallel for num_threads(thread_count) default(none) shared(a, temp, n)
    for (int i = 0; i < n; i++) {
        int count = 0;
        for (int j = 0; j < n; j++) {
            if (a[j] < a[i])
                count++;
            else if (a[j] == a[i] && j < i)
                count++;
        }
        temp[count] = a[i];
    }

    memcpy(a, temp, n * sizeof(int));
    delete[] temp;
}

int main(int argc, char* argv[]) {
    int n;
    cout << "Ingrese el tamaño del arreglo: ";
    cin >> n;

    int* a = new int[n];
    cout << "Ingrese " << n << " números: ";
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }

    Count_sort(a, n, omp_get_max_threads());

    cout << "Arreglo ordenado: ";
    for (int i = 0; i < n; i++) {
        cout << a[i] << " ";
    }
    cout << endl;

    delete[] a;
    return 0;
}
