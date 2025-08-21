#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <pthread.h>

using namespace std;

vector<vector<int>> A;
vector<vector<int>> B;
vector<vector<int>> C;
int numThreads;

vector<vector<int>> generar(int n, int m) {
    vector<vector<int>> mat(n, vector<int>(m));
    srand(time(0));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            mat[i][j] = rand() % 100;
        }
    }
    return mat;
}

void* multWorker(void* arg) {
    int id = *(int*)arg;
    int filas = C.size();
    int columnas = C[0].size();
    int bloque = filas / numThreads;
    int inicio = id * bloque;
    int fin = (id == numThreads - 1) ? filas : inicio + bloque;

    for (int i = inicio; i < fin; i++) {
        for (int j = 0; j < columnas; j++) {
            for (int k = 0; k < (int)A[0].size(); k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return nullptr;
}

int main(){
    int N, M, P;
    cin >> N >> M >> P;
    A = generar(N, M);
    B = generar(M, P);
    C = vector<vector<int>>(N, vector<int>(P, 0));
    numThreads = 4;

    vector<pthread_t> threads(numThreads);
    vector<int> ids(numThreads);

    for (int i = 0; i < numThreads; i++) {
        ids[i] = i;
        pthread_create(&threads[i], nullptr, multWorker, &ids[i]);
    }

    // Esperar que terminen
    for (int i = 0; i < numThreads; i++) {
        pthread_join(threads[i], nullptr);
    }

    cout << "Resultado de la multiplicación:\n";
    for (const auto& fila : C) {
        for (const auto& elem : fila) {
            cout << elem << " ";
        }
        cout << endl;
    }

    return 0;
}